/*
 * A131716  Period 6: repeat [0, 1, 2, 5, 8, 9].
 * https://oeis.org/A131716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A131716: Period 6 repeating [0, 1, 2, 5, 8, 9].
   Formula: a(n) = a(n-6) for n > 5 with a(0)=0, a(1)=1, a(2)=2,
   a(3)=5, a(4)=8, a(5)=9. */
seq_t a(idx_t n)
{
    /* static table of the six repeating terms */
    static const seq_t tbl[6] = {0, 1, 2, 5, 8, 9};
    return tbl[n % 6];
}
