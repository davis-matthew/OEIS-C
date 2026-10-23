/*
 * A156865  a(n) = 729000*n - 612180.
 * https://oeis.org/A156865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 729000*n - 612180
 * Range:     u8 none, u16 none, u32 n <= 5892, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; for n < 1 a dummy zero is returned. */
    if (n == 0)
        return 0;
    return ((seq_t)n * (seq_t)729000) - (seq_t)612180;
}
