/*
 * A172495  a(n) = numerator of fraction whose decimal representation is
 *          (n).(1)(2)(3)...(n-1)(n).
 * https://oeis.org/A172495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) = numerator of fraction whose decimal representation is
 *            (n).(1)(2)(3)...(n-1)(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute 10^e as a seq_t.
   Works for all supported seq_t types (built‑in integers or arbitrary
   precision) because the multiplication operators are defined. */
static seq_t pow10seq(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* n == 0 is undefined for this sequence, but we return 0 for safety. */
    if (n == 0)
        return 0;

    /* K = total number of decimal digits in the concatenation 1…n */
    idx_t K = 0;
    /* S = integer formed by concatenating 1…n */
    seq_t S = 0;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute number of decimal digits of i. */
        idx_t tmp = i;
        int d = 0;
        do
        {   /* at least one iteration; i ≥ 1 */
            tmp /= 10;
            ++d;
        } while (tmp != 0);

        K += (idx_t)d;

        seq_t powdig = pow10seq((idx_t)d);   /* 10^d */
        S = S * powdig + (seq_t)i;
    }

    /* pow10K = 10^K */
    seq_t pow10K = pow10seq(K);

    /* N = n·10^K + S  (the numerator before reduction) */
    seq_t N = (seq_t)n * pow10K + S;

    /* Compute G = gcd(N, 10^K) using Euclid's algorithm */
    seq_t a_val = N;
    seq_t b_val = pow10K;
    while (b_val != 0)
    {
        seq_t t = a_val % b_val;
        a_val = b_val;
        b_val = t;
    }
    seq_t G = a_val;        /* G = gcd(N,10^K) */

    /* Return the reduced numerator N/G */
    return N / G;
}
