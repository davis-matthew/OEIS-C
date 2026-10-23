/*
 * A132705  For an integer n with prime factorization (p_1)*(p_2)*(p_3)* ...
 *          *(p_k), a(n) = (p_1+2)*(p_2+2)*(p_3+2)* ... *(p_k+2).
 * https://oeis.org/A132705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{p^e‖n} (p + 2)^e with a(0)=2, a(1)=3
 * Range:     u8 n <= 15, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n by trial division up to √n, so it is
   O(√n) per evaluation and works for all n ≥ 0. */

#include <stdint.h>
#include <stddef.h>

/* integer square‑root: floor(sqrt(n)) for 0 ≤ n < 2^64  */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t lo = 0, hi = n >> 1;          /* hi = ⌊n/2⌋ is an upper bound */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > n / mid)              /* mid*mid would overflow or exceed n */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Special conventions for 0 and 1 */
    if (n == 0)
        return 2;
    if (n == 1)
        return 3;

    /* Copy of n that will be divided down */
    idx_t m = n;
    seq_t result = 1;

    /* Trial division by all possible divisors up to √m */
    for (idx_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            /* Count the exponent of this prime factor */
            idx_t exp = 0;
            while (m % d == 0) {
                m /= d;
                ++exp;
            }
            /* Compute (d+2)^exp */
            seq_t base = (seq_t)(d + 2);
            seq_t power = 1;
            for (idx_t i = 0; i < exp; ++i)
                power *= base;
            result *= power;
        }
    }

    /* If anything remains, it must be a prime > √original n */
    if (m > 1) {
        seq_t base = (seq_t)(m + 2);
        result *= base;
    }

    return result;
}
