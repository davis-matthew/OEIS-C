/*
 * A056452  a(n) = 6^floor((n+1)/2).
 * https://oeis.org/A056452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^floor((n+1)/2)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute k = floor((n+1)/2) without overflow for large uint64_t values.
       k = (n >> 1) + (n & 1) is equivalent to (n + 1) / 2 for unsigned integers. */
    idx_t k = (n >> 1) + (n & 1);

    seq_t result = 1;
    seq_t base   = 6;

    /* Exponentiation by squaring – efficient and safe for arbitrary‑precision types. */
    while (k)
    {
        if (k & 1)
            result *= base;
        base *= base;
        k >>= 1;
    }

    return result;
}
