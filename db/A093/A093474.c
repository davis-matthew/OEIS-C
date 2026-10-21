/*
 * A093474  Duplicate of A030101.
 * https://oeis.org/A093474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = bit‑reverse of n (ignoring leading zeros)
 * Range:     i8 n <= 28, i16 n <= 28, i32 n <= 28, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t rev = 0;
    idx_t x = n;
    while (x) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}
