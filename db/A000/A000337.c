/*
 * A000337  a(n) = (n-1)*2^n + 1.
 * https://oeis.org/A000337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = (n-1)*2^n + 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = 1;
    idx_t i;
    for (i = 0; i < n; i++)
        p *= 2;
    return (seq_t)n * p + 1 - p;
}
