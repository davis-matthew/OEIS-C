/*
 * A168290  T(n,k) = 5*A046802(n,k) - 4*A007318(n,k), triangle read by rows (0
 *          <= k <= n).
 * https://oeis.org/A168290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = 5 * ( S1(n,k) - S2(n,k) ) - 4 * C(n,k) where S1(n,k) =
 *            Σ_{m=0}^{k} C(n,k-m)*(1+m)^{n-(k-m)}*(-m)^{k-m} S2(n,k) =
 *            Σ_{m=0}^{k-1} C(n,k-m-1)*(1+m)^{n-(k-m-1)}*(-m)^{k-m-1} a(m) is
 *            the triangle entry read by rows (0‑based). The row and column
 *            corresponding to a linear index `m` are obtained...
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binom, pow_seq, signed_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b) */
#include "../utils/pow_seq.h" /* seq_t pow_seq(seq_t a, int k) */
#include "../utils/signed_to_seq.h"

seq_t a(idx_t n)
{
    /* Find row r and column c from linear index n */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;        /* r is the row   */
    idx_t c = n - r * (r + 1) / 2;                   /* c is the column*/

    /* If c == 0 we can return 1 directly */
    if (c == 0)
        return 1;

    /* Accumulate S1 and S2 using signed 128‑bit to avoid overflow */
    __int128 S1 = 0;
    for (idx_t m = 0; m <= c; ++m)
    {
        idx_t mu   = c - m;        /* power of (-m)   */
        seq_t  bin = binom(r, mu); /* binomial coefficient C(r,mu) */

        /* (1+m)^{r-mu}  */
        seq_t pow1 = pow_seq((seq_t)(1 + m), (int)(r - mu));

        /* m^{mu} and the sign from (-m)^{mu} */
        seq_t pow2 = 0;
        if (mu == 0)
            pow2 = 1;
        else
        {
            pow2 = pow_seq((seq_t)m, (int)mu);
            if (mu & 1)          /* odd power → negative */
                pow2 = -pow2;
        }

        __int128 term = (__int128)bin * (__int128)pow1 * (__int128)pow2;
        S1 += term;
    }

    __int128 S2 = 0;
    if (c > 0)
    {
        for (idx_t m = 0; m <= c - 1; ++m)
        {
            idx_t mu   = c - 1 - m;
            seq_t  bin = binom(r, mu);

            seq_t pow1 = pow_seq((seq_t)(1 + m), (int)(r - mu));

            seq_t pow2 = 0;
            if (mu == 0)
                pow2 = 1;
            else
            {
                pow2 = pow_seq((seq_t)m, (int)mu);
                if (mu & 1)
                    pow2 = -pow2;
            }

            __int128 term = (__int128)bin * (__int128)pow1 * (__int128)pow2;
            S2 += term;
        }
    }

    /* Compute T(r,c) and then the sequence element */
    __int128 first_part = 5 * (S1 - S2);
    seq_t   bin_nk      = binom(r, c);
    __int128 final_val  = first_part - 4 * (__int128)bin_nk;

    /* final_val is guaranteed non‑negative for this sequence */
    return signed_to_seq(final_val);
}
