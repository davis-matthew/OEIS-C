/*
 * A119389  Numerator of (1^2/n + 2^2/(n-1) + ... + k^2/(n-k+1) + ... +
 *          (n-1)^2/2 + n^2/1).
 * https://oeis.org/A119389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = Numerator[Sum[k^2/(n-k+1),{k,1,n}]]
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Computes the n-th term of OEIS sequence A119389.
   a(1) = 1, a(2) = 9, a(3) = 34, ...  */
seq_t a(idx_t n)
{
    seq_t num = 0;   /* current numerator of the partial sum */
    seq_t den = 1;   /* current denominator of the partial sum */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* term = k^2 / (n-k+1) */
        seq_t denom_k = (seq_t)(n - k + 1);
        seq_t k2      = (seq_t)k * k;          /* k^2 as seq_t */

        /* add term to current fraction:
           new_num = num * denom_k + den * k2
           new_den = den * denom_k
           reduce by gcd to keep size manageable */
        seq_t new_num = num * denom_k + den * k2;
        seq_t new_den = den * denom_k;

        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* After the loop (num,den) is reduced.
       The desired term is the numerator of the sum. */
    return num;
}
