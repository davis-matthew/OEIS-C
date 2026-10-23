/*
 * A049814  a(n)=number of Farey fractions of order n that are <=1/10; cf.
 *          A049805.
 * https://oeis.org/A049814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{b=2..n} #{ a | 1 ≤ a ≤ ⌊b/10⌋ , gcd(a,b)=1 } (the
 *            0/1 fraction is always included)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   coprime_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/coprime_leq.h"

/* Include definitions of seq_t and idx_t (provided by the build). */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t res = 1; /* count the 0/1 fraction */
    for (idx_t b = 2; b <= n; ++b) {
        idx_t m = b / 10;          /* floor(b/10) */
        if (m == 0) continue;
        res += coprime_leq(b, m);
    }
    return res;
}
