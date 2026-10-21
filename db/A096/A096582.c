/*
 * A096582  From the "100 Green Bottles" song.
 * https://oeis.org/A096582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (n <= 101) ? (101 - n) : 0
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)101)
        return (seq_t)((seq_t)101 - (seq_t)n);
    else
        return (seq_t)0;
}
