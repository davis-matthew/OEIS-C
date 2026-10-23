/*
 * A025778  Expansion of 1/((1-x)(1-x^5)(1-x^8)).
 * https://oeis.org/A025778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor(n*(n+14)/80 + 1)
 * Number of partitions of n into parts 1, 5 and 8.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn + (seq_t)14);
    val = val / (seq_t)80;
    val += (seq_t)1;
    return val;
}
