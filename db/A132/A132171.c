/*
 * A132171  3^n repeated 3^n times.
 * https://oeis.org/A132171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^floor(log_3(2n+1))
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* limit = 2*n + 1, computed in seq_t to avoid overflow */
    seq_t limit = (seq_t)n * 2 + 1;
    seq_t val = 1;
    /* multiply by 3 while the next product would still be <= limit */
    while (val <= limit / 3) {
        val *= 3;
    }
    return val;
}
