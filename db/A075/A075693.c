/*
 * A075693  Difference between 10-adic numbers defined in A018248 & A018247.
 * https://oeis.org/A075693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, base
 * Formula:   a(n) = A018248(n) - A018247(n) (difference of the two 10‑adic
 *            automorphic numbers)
 * Range:     i8 n <= 9999, i16 n <= 9999, i32 n <= 9999, i64 n <= 9999,
 *            i128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    size_t N = (size_t)n;
    /* arrays for digits of the 5‑adic and 6‑adic automorphic numbers (LSB first) */
    unsigned char *d5 = (unsigned char*)malloc(N + 1);
    unsigned char *d6 = (unsigned char*)malloc(N + 1);
    /* arrays for coefficients of square a² modulo 10^(k+1) (digits with carries already applied) */
    int *sq5 = (int*)calloc(2 * N + 5, sizeof(int));
    int *sq6 = (int*)calloc(2 * N + 5, sizeof(int));

    /* initial digit: 5‑adic starts with 5, 6‑adic starts with 6 */
    d5[0] = 5;
    d6[0] = 6;

    /* initialise the square‑coefficient arrays for the 1‑digit numbers */
    {
        int val = 5 * 5;                /* 5² = 25 */
        sq5[0] = val % 10;              /* units digit of the square */
        int carry = val / 10;           /* ten’s digit of the square */
        if (carry > 0) sq5[1] += carry;
    }
    {
        int val = 6 * 6;                /* 6² = 36 */
        sq6[0] = val % 10;
        int carry = val / 10;
        if (carry > 0) sq6[1] += carry;
    }

    /* extend digits up to the Nth (0‑based) position */
    for (size_t k = 1; k <= N; ++k)
    {
        /* 5‑adic digit */
        int d5k = sq5[k] % 10;          /* d = coefficient of 10^k in the square   */
        d5[k] = (unsigned char)d5k;

        /* update the square coefficients for the 5‑adic number
         * a' = a + d5k * 10^k   =>   a'^2 = a^2 + 2*a*d5k*10^k + d5k²*10^(2k)   */
        for (size_t i = 0; i < k; ++i)
            sq5[i + k] += 2 * d5[i] * d5k;
        sq5[2 * k] += d5k * d5k;
        /* propagate carries from position k onward */
        int carry = 0;
        for (size_t pos = k; pos <= 2 * k + 2; ++pos)
        {
            int tmp = sq5[pos] + carry;
            sq5[pos] = tmp % 10;
            carry    = tmp / 10;
        }

        /* 6‑adic digit:  d ≡ 9 * m (mod 10),  where m = coefficient of 10^k in a²
           (because for 6‑adic, 1‑2*a ≡ 9 (mod 10) and its inverse modulo 10 is 9)   */
        int m6 = sq6[k] % 10;
        int d6k = (9 * m6) % 10;
        d6[k] = (unsigned char)d6k;

        /* update the square coefficients for the 6‑adic number */
        for (size_t i = 0; i < k; ++i)
            sq6[i + k] += 2 * d6[i] * d6k;
        sq6[2 * k] += d6k * d6k;
        carry = 0;
        for (size_t pos = k; pos <= 2 * k + 2; ++pos)
        {
            int tmp = sq6[pos] + carry;
            sq6[pos] = tmp % 10;
            carry    = tmp / 10;
        }
    }

    /* result is the difference of the nth digits (n ≥ 1) */
    seq_t result = (seq_t)d6[N] - (seq_t)d5[N];

    /* free temporary storage */
    free(d5); free(d6); free(sq5); free(sq6);

    return result;
}
