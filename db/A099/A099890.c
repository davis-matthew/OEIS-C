/*
 * A099890  XOR BINOMIAL transform of the odd numbers; also the main diagonal
 *          of the XOR difference triangle A099889.
 * https://oeis.org/A099890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n == 0 * a(n) = 2^(k+1) if n is a non‑zero power of
 *            two (n = 2^k) * a(n) = 0 otherwise * (shift amount is k+1, where
 *            k = log2(n))
 * Range:     u8 n <= 127, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the offset */
    if (n == 0)
        return (seq_t)1;

    /* Check if n is a power of two (and non‑zero) */
    if ((n & (n - 1)) == 0) {
        /* Compute k = log2(n) by counting trailing zeros / divisions by 2 */
        idx_t k = 0;
        idx_t t = n;
        while (t > 1) {
            t >>= 1;
            ++k;
        }

        /* Determine how many bits our seq_t type has */
        size_t bits = sizeof(seq_t) * 8U;

        /* If the desired shift is not representable, return 0 (value too large) */
        if (bits <= (size_t)(k + 1))
            return (seq_t)0;

        /* Compute 2^(k+1) */
        return ((seq_t)1) << (k + 1);
    }

    /* All other indices yield zero */
    return (seq_t)0;
}
