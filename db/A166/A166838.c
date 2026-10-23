/*
 * A166838  Number of nX2 1..4 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          decreasing order.
 * https://oeis.org/A166838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 18*n^5 + 100*n^4 - 30*n^3 - 551*n^2 + 462*n)/180
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 37, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x3 * x;
    seq_t x5 = x4 * x;
    seq_t x6 = x5 * x;

    seq_t numerator =
        x6
        + (seq_t)18 * x5
        + (seq_t)100 * x4
        - (seq_t)30  * x3
        - (seq_t)551 * x2
        + (seq_t)462 * x;

    return numerator / (seq_t)180;
}
