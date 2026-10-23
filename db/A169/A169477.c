/*
 * A169477  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32 * 31^(n-1), a(0) = 1 * (Number of reduced words of
 *            length n in a Coxeter group with 32 generators * and relations
 *            (s_i)^2 = (s_i s_j)^3 = I.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* For big_pow(base, exp) */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)        /* a(0) = 1 */
        return (seq_t)1;

    /* Compute 31^(n-1) and multiply by 32.
     * big_pow works for all seq_t types (uint*_t, unsigned __int128,
     * or arbitrary‑precision integer classes). */
    seq_t pow31 = big_pow((seq_t)31, (seq_t)(n - (idx_t)1));
    return pow31 * (seq_t)32;
}
