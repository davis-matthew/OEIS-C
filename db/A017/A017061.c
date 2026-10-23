/*
 * A017061  a(n) = (7*n + 6)^9.
 * https://oeis.org/A017061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 6)^9
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 18, u128 n <= 2729,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * 7 + 6;   /* (7*n + 6) cast to seq_t */
    seq_t res  = 1;                  /* result accumulator */
    for (idx_t i = 0; i < 9; ++i)
        res *= base;                /* multiply base nine times */
    return res;
}
