/*
 * A000312  a(n) = n^n; number of labeled mappings from n points to themselves
 *          (endofunctions).
 * https://oeis.org/A000312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = n^n, with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 1;
    seq_t b = (seq_t)n;
    idx_t e = n;
    while (e > 0) {
        if (e & 1)
            r *= b;
        e >>= 1;
        if (e > 0)
            b *= b;
    }
    return r;
}
