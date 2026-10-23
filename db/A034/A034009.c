/*
 * A034009  Convolution of A000295(n+2) (n>=0) with itself.
 * https://oeis.org/A034009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n+4)*(n-3) + ((n+7)*(n*(n+11)+42))/6
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 23, u64 n <= 54, u128 n <= 117,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t once to avoid repeated casts. */
    seq_t nn = (seq_t)n;

    /* 2^(n+4) as seq_t. */
    seq_t pow2_n4 = pow2(n + 4);

    /* Compute the second part K = (n+7)*(n*(n+11)+42)/6. */
    seq_t tmp = nn * (nn + 11) + (seq_t)42;          /* n*(n+11)+42 */
    seq_t K    = (nn + 7) * tmp / (seq_t)6;          /* ((n+7)*tmp)/6 */

    if (n < 3)
    {
        /* For n = 0,1,2 the term 2^(n+4)*(n-3) is negative.
           Compute as subtraction: a = K - 2^(n+4)*(3-n). */
        seq_t subtract = pow2_n4 * ((seq_t)(3 - n));
        return K - subtract;
    }
    else
    {
        /* For n ≥ 3 the term is non‑negative. */
        seq_t term1 = pow2_n4 * (nn - 3);
        return term1 + K;
    }
}
