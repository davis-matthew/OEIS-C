/*
 * A220466  a((2*n-1)*2^p) = 4^p*(n-1) + 2^(p-1)*(1+2^p), p >= 0 and n >= 1.
 * https://oeis.org/A220466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a((2*n-1)*2^p) = 4^p*(n-1) + 2^(p-1)*(1+2^p) (p>=0, n>=1)
 *            Derived from the odd‑part representation of each index
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the OEIS sequence A220466 entry for index n (n>=1).  */
seq_t a(idx_t n)
{
    /* Find p = exponent of 2 dividing n and the odd part m. */
    idx_t p = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {
        temp >>= 1;
        p++;
    }
    idx_t m = temp;              /* odd part of n   */
    idx_t n0 = (m + 1) >> 1;     /* n0 = (odd+1)/2  */

    /* For p = 0 the formula simplifies to a(n) = n0. */
    if (p == 0)
        return (seq_t)n0;

    /* Compute powers of 2 using shifts (safe for the expected n range). */
    seq_t two_pow_p       = ((seq_t)1 << p);       /* 2^p          */
    seq_t two_pow_2p      = ((seq_t)1 << (2*p));   /* 4^p = 2^(2p) */
    seq_t two_pow_p_minus = ((seq_t)1 << (p-1));   /* 2^(p-1)      */

    /* Evaluate the formula: 4^p*(n0-1) + 2^(p-1)*(1+2^p). */
    seq_t part1 = two_pow_2p * (seq_t)(n0 - 1);
    seq_t part2 = two_pow_p_minus * (seq_t)(1 + two_pow_p);

    return part1 + part2;
}
