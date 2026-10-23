/*
 * A016784  a(n) = (3*n+1)^8.
 * https://oeis.org/A016784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 4, u64 n <= 84, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes (3*n+1)^8 for any non-negative index n.
   Works for seq_t being an unsigned integer type (8,16,32,64,128 bits)
   or an arbitrary-precision integer class supporting the usual
   arithmetic operators. */
seq_t a(idx_t n)
{
    /* base = 3*n + 1; all operations are in seq_t to avoid
       overflow before the final result is produced. */
    seq_t base = (seq_t)3 * (seq_t)n + (seq_t)1;

    /* Compute base^8 by repeated multiplication. */
    seq_t val = (seq_t)1;
    for (unsigned int i = 0; i < 8; ++i)
        val *= base;

    return val;
}
