/*
 * A047668  Row 4 of array in A047666.
 * https://oeis.org/A047668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 8*n^2 + 3) / 3
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A047668: Row 4 of array in A047666.  offset 1. */

seq_t a(idx_t n)
{
    /* cast n to seq_t to preserve precision for all supported seq_t types */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2  */
    seq_t n4 = n2 * n2;          /* n^4  */
    seq_t sum = n4 + 8 * n2 + 3; /* n^4 + 8*n^2 + 3 */
    return sum / 3;              /* divide by 3 (exact division) */
}
