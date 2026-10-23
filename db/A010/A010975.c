/*
 * A010975  a(n) = binomial(n,22).
 * https://oeis.org/A010975
 *
 * Offset:    22    (a(22) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,22)
 * Range:     u8 n <= 23, u16 n <= 26, u32 n <= 36, u64 n <= 78, u128 n <= 521,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 22.  For smaller n
       the result is undefined; we return 0. */
    if (n < 22) return 0;

    /* Initial denominator factors 1,2,...,22.  They will be
       cancelled against the numerator factors. */
    static const idx_t denom_init[22] = {
        1,  2,  3,  4,  5,  6,  7,  8,
        9, 10, 11, 12, 13, 14, 15, 16,
        17,18,19,20,21,22
    };
    idx_t denom[22];
    for (int i = 0; i < 22; ++i) denom[i] = denom_init[i];

    seq_t result = 1;

    /* Multiply by each numerator term (n, n-1, ..., n-21),
       cancelling common factors with the denominator. */
    for (idx_t i = 0; i < 22; ++i)
    {
        idx_t num = n - i;

        /* Cancel with denominator factors. */
        for (int j = 0; j < 22 && num > 1; ++j)
        {
            idx_t g = gcd_idx(num, denom[j]);
            if (g > 1)
            {
                num /= g;
                denom[j] /= g;
            }
        }

        /* Multiply by the remaining numerator part. */
        result = result * (seq_t)num;
    }

    /* Divide by any remaining denominator factors (they must all be
       1 at this point except for those that were not cancelled
       because the product is an integer). */
    for (int j = 0; j < 22; ++j)
    {
        if (denom[j] > 1)
            result = result / (seq_t)denom[j];
    }

    return result;
}
