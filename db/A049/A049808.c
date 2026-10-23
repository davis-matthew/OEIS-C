/*
 * A049808  a(n)=number of Farey fractions of order n that are <=1/4; cf.
 *          A049805.
 * https://oeis.org/A049808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{b=1}^{n} #{ a | 1 ≤ a ≤ ⌊b/4⌋ , gcd(a,b)=1 }
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 0/1 counts as 1 for all n ≥ 1. */

#include <stdint.h>

/* Euclid's algorithm for gcd, used on small integers (≤ n). */
static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1; return 0 safely */

    seq_t count = 1;              /* count 0/1 */

    for (idx_t b = 1; b <= n; ++b) {
        idx_t max_a = b / 4;      /* floor(b/4) */
        for (idx_t a_num = 1; a_num <= max_a; ++a_num) {
            if (gcd_uint64(a_num, b) == 1)
                ++count;
        }
    }

    return count;
}
