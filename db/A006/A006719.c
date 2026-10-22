/*
 * A006719  a(n) = A000905(n) + 1.
 * https://oeis.org/A006719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000905(n) + 1, where A000905 (Hamilton numbers)
 *            satisfies H(1) = 2, H(m) = 2 + Sum_{k=1..m-1} (-1)^(k+1) *
 *            binomial(H(m-k), k+1)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   binx, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/binx.h"

static seq_t *H = 0;
static idx_t Hlen = 0; /* H[0..Hlen-1] allocated; H[1..Hcnt] valid */
static idx_t Hcnt = 0;

seq_t a(idx_t n)
{
    if (n < 1) n = 1;
    if (n + 1 > Hlen) {
        idx_t nl = (n + 1 > 2 * Hlen) ? n + 1 : 2 * Hlen;
        H = grow_zero(H, Hlen, nl);
        Hlen = nl;
    }
    if (Hcnt == 0) {
        H[1] = 2;
        Hcnt = 1;
    }
    while (Hcnt < n) {
        idx_t m = Hcnt + 1;
        seq_t pos = 2, neg = 0;
        idx_t k;
        for (k = 1; k <= m - 1; k++) {
            seq_t t = binx(H[m - k], k + 1);
            if (k % 2 == 1) pos += t;
            else neg += t;
        }
        H[m] = pos - neg;
        Hcnt = m;
    }
    return H[n] + 1;
}
