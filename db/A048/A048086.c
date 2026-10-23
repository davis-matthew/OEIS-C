/*
 * A048086  Number of nonempty subsets of {1,2,...,n} in which exactly 3/4 of
 *          the elements are <= (n+3)/3.
 * https://oeis.org/A048086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} C(floor((n+3)/3), 3k) * C(n – floor((n+3)/3), k)
 *            where the sum runs while 3k ≤ floor((n+3)/3) and k ≤ n –
 *            floor((n+3)/3)
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t binom_c(idx_t n, idx_t r)
{
    if (r > n) return 0;
    if (r > n - r) r = n - r;
    seq_t res = 1;
    for (idx_t i = 1; i <= r; ++i) {
        seq_t num   = (seq_t)(n - r + i);
        seq_t denom = (seq_t)i;
        seq_t g1 = gcd(res, denom);
        res /= g1;
        denom /= g1;
        seq_t g2 = gcd(num, denom);
        num /= g2;
        denom /= g2;               /* denom should now be 1 */
        res *= num;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t m = (n + 3) / 3;               /* floor((n+3)/3)            */
    idx_t r = n - m;                     /* remaining elements         */
    idx_t maxk = m / 3;                  /* 3k ≤ m                     */
    if (r < maxk) maxk = r;              /* k ≤ r                       */
    seq_t total = 0;

    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t c1 = binom_c(m, 3 * k);
        seq_t c2 = binom_c(r, k);
        total += c1 * c2;
    }

    return total;
}
