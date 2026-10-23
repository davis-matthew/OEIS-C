/*
 * A017799  Binomial coefficients C(83,n).
 * https://oeis.org/A017799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(83,n) (row 83 of Pascal's triangle)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 20, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute gcd for seq_t values.  Uses the Euclidean algorithm. */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    const idx_t N = 83;
    if (n > N)
        return 0;                     /* C(83,n) = 0 for n > 83 */

    idx_t k = n;
    if (k > N - k)                    /* use symmetry C(83,n) = C(83,83-n) */
        k = N - k;

    seq_t result = 1;

    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t numerator = (seq_t)(N - k + i);
        seq_t curr_i    = (seq_t)i;

        /* Cancel factors of curr_i with the current result */
        seq_t g = seq_gcd(result, curr_i);
        if (g > 1)
        {
            result /= g;
            curr_i /= g;
        }

        /* Then cancel remaining factors of curr_i with the numerator */
        g = seq_gcd(numerator, curr_i);
        if (g > 1)
        {
            numerator /= g;
            curr_i /= g;
        }

        /* At this point curr_i should be 1; multiply the reduced numerator */
        result *= numerator;
    }

    return result;
}
