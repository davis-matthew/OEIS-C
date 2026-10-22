/*
 * A006214  Number of down-up permutations of n+5 starting with n+1.
 * https://oeis.org/A006214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..floor((n-1)/2)}
 *            (-1)^i*binomial(n,2i+1)*E(n+3-2i), where E(j) = A000111(j) are
 *            the Euler numbers; computed by the boustrophedon (Entringer)
 *            recurrence E(m,k) = E(m,k-1) + E(m-1,m-k), E(0,0) = 1, E(m,0) =
 *            0, E(m) = E(m,m)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *eul = NULL;   /* eul[j] = E(j), j < elen */
static seq_t *row = NULL;   /* Entringer row elen-1 */
static seq_t *tmp = NULL;
static idx_t ecap = 0, elen = 0;

static void ensure_euler(idx_t N) {
  if (elen > N) return;
  if (N + 1 > ecap) {
    idx_t newcap = 2 * ecap;
    if (newcap < N + 1) newcap = N + 1;
    seq_t *ne = alloc_zero(newcap);
    seq_t *nr = alloc_zero(newcap);
    seq_t *nt = alloc_zero(newcap);
    for (idx_t j = 0; j < elen; ++j) {
      ne[j] = eul[j];
      nr[j] = row[j];
    }
    if (eul) {
      release(eul, ecap);
      release(row, ecap);
      release(tmp, ecap);
    }
    eul = ne; row = nr; tmp = nt;
    ecap = newcap;
  }
  if (elen == 0) {
    row[0] = 1;
    eul[0] = 1;
    elen = 1;
  }
  while (elen <= N) {
    idx_t m = elen;
    tmp[0] = 0;
    for (idx_t k = 1; k <= m; ++k)
      tmp[k] = tmp[k - 1] + row[m - k];
    seq_t *sw = row; row = tmp; tmp = sw;
    eul[m] = row[m];
    ++elen;
  }
}

seq_t a(idx_t n) {
  if (n == 0) return 0;
  ensure_euler(n + 3);

  seq_t pos = 0, neg = 0;
  seq_t c = (seq_t)n;               /* binomial(n, k), k = 2i+1 */
  for (idx_t i = 0; 2 * i + 1 <= n; ++i) {
    idx_t k = 2 * i + 1;
    seq_t term = c * eul[n + 3 - 2 * i];
    if (i % 2 == 0) pos += term;
    else neg += term;
    if (k + 2 <= n) {
      c = c * (seq_t)(n - k) / (seq_t)(k + 1);
      c = c * (seq_t)(n - k - 1) / (seq_t)(k + 2);
    }
  }
  return pos - neg;
}
