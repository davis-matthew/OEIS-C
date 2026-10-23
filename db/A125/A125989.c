/*
 * A125989  Sum of heights of 10's in binary expansion of n.
 * https://oeis.org/A125989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = sum of the heights of all occurrences of the binary
 *            pattern "10" in the binary expansion of n. The height is updated
 *            from the least significant bit toward the most significant:
 *            start at 0, add 1 for each 0‑bit and subtract 1 for each 1‑bit.
 *            When a 1‑bit follows a 0‑bit (i.e. a "10" pattern...
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum    = 0;
    seq_t height = 0;
    int  prevBit = 0;           /* assume a leading 0 before first bit */
    idx_t x = n;

    while (x > 0)
    {
        int bit = x & 1ULL;

        if (bit == 1 && prevBit == 0)
            sum += height;

        if (bit == 1)
            height -= 1;
        else
            height += 1;

        prevBit = bit;
        x >>= 1;
    }

    return sum;
}
