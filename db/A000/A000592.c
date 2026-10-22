/*
 * A000592  Number of nonnegative solutions of x^2 + y^2 = z in first n
 *          shells.
 * https://oeis.org/A000592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{j <= t_n} n_2(j), where t_n is the n-th (0-based)
 *            integer that is a sum of two squares (i.e. the n-th shell,
 *            n_2(t_n) > 0) and n_2(j) = #{(x,y) >= 0 : x^2 + y^2 = j}; n_2 is
 *            computed by a sieve
 * Range:     u8 n <= 113, u16 n <= 2749, u32 n <= 2749, u64 n <= 2749,
 *            u128 n <= 2749, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *cnt = NULL;
static uint64_t cnt_lim = 0;

static void build(uint64_t L) {
  uint64_t x, y;
  free(cnt);
  cnt = (uint32_t *)calloc((size_t)(L + 1), sizeof(uint32_t));
  for (x = 0; x * x <= L; x++)
    for (y = 0; x * x + y * y <= L; y++)
      cnt[x * x + y * y]++;
  cnt_lim = L;
}

seq_t a(idx_t n) {
  uint64_t L = cnt_lim ? cnt_lim : 1024;
  for (;;) {
    uint64_t z, shells = 0;
    seq_t sum = 0;
    if (cnt_lim < L) build(L);
    for (z = 0; z <= cnt_lim; z++) {
      if (cnt[z]) {
        sum += (seq_t)cnt[z];
        if (shells == (uint64_t)n) return sum;
        shells++;
      }
    }
    L = cnt_lim * 2;
  }
}
