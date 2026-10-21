/*
 * A227772  Sequence based on factorial representation converging to 1 in
 *          2-adic numbers, and 0 in p-adic numbers for any other p.
 * https://oeis.org/A227772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that * m ≡ 1 (mod 2^v2(n!)) and * m ≡ 0
 *            (mod odd_part(n!)), * where v2(n!) is the exponent of 2 in n!
 *            and odd_part(n!) is * the product of all odd factors of n!. *
 *            For n = 1 the sequence value is 0
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1) return 0;

    /* Compute v2(n!) = sum_{k>=1} floor(n/2^k) */
    idx_t v2 = 0;
    for (idx_t k = 2; k <= n; k <<= 1) v2 += n / k;

    /* Compute odd part of n! : product of i after removing all factors of 2 */
    seq_t odd = 1;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t x = i;
        while ((x & 1ULL) == 0) x >>= 1;   /* strip powers of two */
        odd *= (seq_t)x;
    }

    /* Compute inverse of odd modulo 2^v2 using Hensel lifting */
    seq_t inv = 1;                    /* inverse modulo 2 */
    seq_t mod_next = 2;               /* current modulus 2^1 */

    for (idx_t k = 1; k < v2; ++k) {
        seq_t mask   = (mod_next << 1) - 1;        /* 2^(k+1) - 1 */
        seq_t a_mod  = odd & mask;                 /* odd mod 2^(k+1) */
        seq_t term   = (a_mod * inv) & mask;       /* odd * inv mod 2^(k+1) */

        seq_t inv_c = 2;                           /* 2 - term */
        inv_c -= term;                             /* unsigned underflow OK */
        inv_c &= mask;                             /* keep mod 2^(k+1) */

        inv = (inv * inv_c) & mask;                /* new inverse mod 2^(k+1) */
        mod_next <<= 1;                            /* next modulus 2^(k+1) */
    }

    /* Result: odd_part * inverse = a(n) */
    return odd * inv;
}
