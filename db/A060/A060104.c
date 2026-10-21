/*
 * A060104  Fifth column (m=4) of triangle A060102.
 * https://oeis.org/A060104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^2+10*n+7)*binomial(n+6,6)/7
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"

seq_t a(idx_t n)
{
    /* compute 2*n*n + 10*n + 7 using 128‑bit to avoid overflow */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 t1   = 2*nn*nn + 10*nn + 7;

    /* binomial(n+6,6) is provided by the helper */
    seq_t bin = binom_n_k(n + 6, 6);

    /* multiply, divide by 7, then cast back to seq_t */
    unsigned __int128 prod = t1 * (unsigned __int128)bin;
    prod /= 7;
    return (seq_t)prod;
}
