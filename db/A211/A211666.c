/*
 * A211666  Number of iterations log_10(log_10(log_10(...(n)...))) such that
 *          the result is < 2.
 * https://oeis.org/A211666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 0, if n == 1; 1 if 2 <= n < 100; 2 otherwise
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the number of iterations of log_10 applied to n
 * until the result becomes < 2, with n being the index of the sequence. */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n < 100) return (seq_t)1;
    return (seq_t)2;
}
