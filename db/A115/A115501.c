/*
 * A115501  Number of monic irreducible polynomials of degree 2 in
 *          GF(2^n)[x1,x2,x3,x4].
 * https://oeis.org/A115501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = q^5 * (1 + q + q^2 + ... + q^9) - (q^4 + q^3 + q^2 + q) * (q^4 + q^3 + q^2 + q + 1) / 2
 * where q = 2^n (field size)
 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* q = 2^n */
    seq_t one = (seq_t)1;
    /* powers of 2: 2^(k*n) */
    /* Construct S = 1 + q + q^2 + ... + q^9 */
    seq_t S = one;                     /* 2^0 */
    for (idx_t i = 1; i <= 9; ++i)
    {
        S += (one << (i * n));         /* + 2^(i*n) */
    }

    /* Construct L = q^4 + q^3 + q^2 + q = 2^(4n) + 2^(3n) + 2^(2n) + 2^(n) */
    seq_t L = (one << (4 * n)) + (one << (3 * n)) + (one << (2 * n)) + (one << n);

    /* L1 = L + 1 (for (L+1) factor) */
    seq_t L1 = L + one;

    /* a2 = S * 2^(5n)  -> shift S left by 5*n */
    seq_t a2 = S << (5 * n);

    /* reducible = L * (L+1) / 2  -> (L*L1) >> 1 */
    seq_t reducible = (L * L1) >> 1;

    /* final result */
    return a2 - reducible;
}
