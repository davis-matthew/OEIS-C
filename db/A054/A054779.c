/*
 * A054779  a(n) = 6n*(6n-1)*(6n-2)*(6n-3)*(6n-4)*(6n-5).
 * https://oeis.org/A054779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6n*(6n-1)*(6n-2)*(6n-3)*(6n-4)*(6n-5)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t once to avoid repeated promotion */
    seq_t six_n = (seq_t)6 * (seq_t)n;
    seq_t v0 = six_n;
    seq_t v1 = six_n - 1;
    seq_t v2 = six_n - 2;
    seq_t v3 = six_n - 3;
    seq_t v4 = six_n - 4;
    seq_t v5 = six_n - 5;
    /* product of the six consecutive terms */
    return v0 * v1 * v2 * v3 * v4 * v5;
}
