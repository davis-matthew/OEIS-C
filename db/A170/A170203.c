/*
 * A170203  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n==0; otherwise a(n) = 50 * 49^(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_pow.h"   /* for seq_pow */
#include "../utils/seq_mul.h"   /* for seq_mul */

/* seq_t and idx_t are defined by the build */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* compute 49^(n-1) */
    seq_t pow49 = seq_pow((seq_t)49, n - 1);

    /* multiply by 50 to get a(n) */
    return seq_mul((seq_t)50, pow49);
}
