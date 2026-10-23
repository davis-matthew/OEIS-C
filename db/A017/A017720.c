/*
 * A017720  Binomial coefficients C(n,56).
 * https://oeis.org/A017720
 *
 * Offset:    56    (a(56) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,56)
 * Range:     u8 n <= 57, u16 n <= 59, u32 n <= 63, u64 n <= 78, u128 n <= 134,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Return the n‑th term of OEIS sequence A017720.
 * Defined for n ≥ 56; for n < 56 the function returns 0 (offset not used). */
seq_t a(idx_t n)
{
    const idx_t K = 56;
    if (n < K) return (seq_t)0;

    seq_t res = (seq_t)1;

    for (idx_t i = 1; i <= K; ++i)
    {
        /* Current numerator (n - K + i) and denominator (i) */
        seq_t num = (seq_t)(n - K + i);
        seq_t denom = (seq_t)i;

        /* Remove common factors between res and the denominator to keep numbers small */
        seq_t g1 = gcd(res, denom);
        res /= g1;
        denom /= g1;

        /* Remove common factors between the numerator and the remaining denominator */
        seq_t g2 = gcd(num, denom);
        num /= g2;
        denom /= g2;   /* denom should be 1 here */

        /* Multiply with the reduced numerator */
        res *= num;
    }

    return res;
}
