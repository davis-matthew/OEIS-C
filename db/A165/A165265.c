/*
 * A165265  Concatenate the number of 1's and number of 0's in the binary
 *          expansion of n.
 * https://oeis.org/A165265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = concat(number of 1-bits in n, number of 0-bits in n) The
 *            value is computed as a(n) = ones * 10^d + zeros, where d is the
 *            number of decimal digits of zeros
 * Range:     u8 n <= 4096, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* only needed for the header that may be used by the harness */

/* Count 1-bits in n. */
static inline idx_t popcount_idx(idx_t n)
{
    idx_t c = 0;
    while (n)
    {
        c += n & 1U;
        n >>= 1U;
    }
    return c;
}

/* Length of the binary representation of n (1 for n==0). */
static inline idx_t bitlen_idx(idx_t n)
{
    if (n == 0)
        return 1U;
    idx_t len = 0;
    while (n)
    {
        len += 1U;
        n >>= 1U;
    }
    return len;
}

/* Number of decimal digits of a non‑negative integer x. */
static inline idx_t digits_of_idx(idx_t x)
{
    if (x == 0U)
        return 1U;
    idx_t d = 0;
    while (x)
    {
        d += 1U;
        x /= 10U;
    }
    return d;
}

/* Compute 10^e as a seq_t (e is small, e <= 2 for 64‑bit indices). */
static inline seq_t pow10_seq(idx_t e)
{
    seq_t p = 1U;
    for (idx_t i = 0U; i < e; ++i)
    {
        p *= 10U;
    }
    return p;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t ones         = popcount_idx(n);
    idx_t bits         = bitlen_idx(n);
    idx_t zeros        = bits - ones;
    idx_t zeros_digits = digits_of_idx(zeros);
    seq_t pow10        = pow10_seq(zeros_digits);
    seq_t result       = (seq_t)ones * pow10 + (seq_t)zeros;
    return result;
}
