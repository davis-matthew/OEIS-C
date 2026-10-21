/*
 * A074991  Concatenation of n, n+1, n+2 divided by 3.
 * https://oeis.org/A074991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (concatenate(n, n+1, n+2)) / 3
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   dec_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/dec_digits_idx.h"

/* Helper to compute 10^e as a seq_t.  The exponent e is small (at most
   the total number of decimal digits of n+2). */
static seq_t pow10_idx(idx_t e)
{
    seq_t p = 1;
    for (idx_t i = 0; i < e; ++i)
        p *= 10;
    return p;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t n1 = n + 1;
    idx_t n2 = n + 2;

    /* Number of decimal digits of each term. */
    int len1 = dec_digits_idx(n);
    int len2 = dec_digits_idx(n1);
    int len3 = dec_digits_idx(n2);

    /* 10^(len2+len3) and 10^len3 as seq_t. */
    seq_t pow1 = pow10_idx((idx_t)(len2 + len3));
    seq_t pow2 = pow10_idx((idx_t)len3);

    /* Concatenation: n * 10^(len2+len3) + (n+1) * 10^len3 + (n+2) */
    seq_t concat = (seq_t)n * pow1;
    concat += (seq_t)n1 * pow2;
    concat += (seq_t)n2;

    /* The concatenated number is always divisible by 3. */
    return concat / 3;
}
