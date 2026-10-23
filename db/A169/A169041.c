/*
 * A169041  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = 28 * 27^(n‑1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the number of length‑n words over a 28‑symbol alphabet
   with no two identical consecutive generators – the number of reduced
   words in the Coxeter group described. */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(0) is the empty word */
    if (n == 0)
        return (seq_t)1;

    /* compute 27^(n-1) using the provided generic exponentiation,
       then multiply the result by 28.  All operations are performed
       in seq_t so we get exact integers for any supported precision. */
    seq_t pow = seq_pow((seq_t)27, n - 1);
    pow *= (seq_t)28;
    return pow;
}
