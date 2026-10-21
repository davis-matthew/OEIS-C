/*
 * A055454  a(n) = A055450(n, n-4).
 * https://oeis.org/A055454
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A055450(n, n-4) * Implemented via the hypergeometric
 *            closed form: * a(n) = 42 * _2F1([8‑n, 11/2], [7], -4) for n ≥ 8
 *            * The calculation is performed using integer arithmetic only. *
 *            For 4 ≤ n ≤ 7 the values are supplied by a small lookup table
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 20, u64 n <= 34, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm for seq_t (assumes seq_t supports %, /, ==). */
static inline seq_t my_gcd(seq_t a, seq_t b)
{
    while ((seq_t)0 != b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Hard‑coded terms for n = 4 … 7 (offset 4).  These are the only terms
     * below the general hypergeometric formula that are needed in the
     * domain of our implementation. */
    static const seq_t small[4] = {1, 5, 14, 28};
    if (n < 4) return 0;                     /* undefined below offset */
    if (n <= 7) return small[n - 4];

    /* n ≥ 8 */
    int64_t offset = (int64_t)n - 8;          /* n - 8, non‑negative        */
    idx_t   m      = (idx_t)offset;           /* number of extra terms      */
    seq_t term_num = 42;                      /* numerator of current term  */
    seq_t term_den = 1;                       /* denominator (always 1)     */
    seq_t sum      = term_num;                /* running total              */

    for (idx_t k = 1; k <= m; ++k) {
        /* Components for the current ratio:
         *   absFactor1 = offset - (k-1)   (positive)
         *   factor2   = 11 + 2*(k-1)
         *   num4      = 2                  (from 4/2 simplification)
         *   den1      = k + 6              (7 + k - 1)
         *   den2      = k
         */
        seq_t absFactor1 = (seq_t)(offset - (k - 1));
        seq_t factor2    = (seq_t)(11 + 2 * (k - 1));
        seq_t num4       = (seq_t)2;
        seq_t den1       = (seq_t)(k + 6);
        seq_t den2       = (seq_t)k;

        /* Reduce the numerator factors against the denominator factors */
        seq_t g;

        g = my_gcd(absFactor1, den1);
        absFactor1 /= g;  den1 /= g;

        g = my_gcd(absFactor1, den2);
        absFactor1 /= g;  den2 /= g;

        g = my_gcd(factor2, den1);
        factor2 /= g;   den1 /= g;

        g = my_gcd(factor2, den2);
        factor2 /= g;   den2 /= g;

        g = my_gcd(num4, den1);
        num4 /= g;   den1 /= g;

        g = my_gcd(num4, den2);
        num4 /= g;   den2 /= g;

        /* Reduce the current term's numerator against any remaining
         * denominator factors.  After this reduction the overall
         * denominator of the new term will be 1. */
        g = my_gcd(term_num, den1);
        term_num /= g;  den1 /= g;

        g = my_gcd(term_num, den2);
        term_num /= g;  den2 /= g;

        /* Build the new term (still an integer).  Any residual
         * common factor between numerator and denominator is eliminated
         * by the final gcd calculation. */
        term_num = term_num * absFactor1 * factor2 * num4;
        term_den = den1 * den2;

        g = my_gcd(term_num, term_den);
        term_num /= g;  term_den /= g;   /* term_den should be 1 now */

        /* Add the new term to the running sum.  The term is guaranteed
         * to be an integer. */
        sum += term_num;
    }

    return sum;
}
