/*
 * A051033  a(n) = binomial(n, floor(n/3)).
 * https://oeis.org/A051033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n, floor(n/3))
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 74, u128 n <= 143,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Number of elements to choose */
    idx_t k = n / 3;
    /* Use the smaller of k and n-k to keep loop small */
    if (k > n - k)
        k = n - k;

    seq_t result = 1;

    /* Multiply numerators and divide by denominators step‑by‑step,
       reducing with gcds to avoid overflow. */
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = n - k + i; /* numerator element */
        seq_t den = i;         /* denominator element */

        /* Reduce num/den pair */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        /* Reduce result with the remaining denominator factor */
        g = gcd(result, den);
        result /= g;
        den /= g;

        /* Update result */
        result = result * num;
    }

    return result;
}
