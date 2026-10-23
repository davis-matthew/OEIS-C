/*
 * A017286  a(n) = (10*n + 1)^6.
 * https://oeis.org/A017286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 1)^6
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 162, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t for correct computation */
    seq_t base = ((seq_t)n * (seq_t)10) + 1;
    seq_t res  = base * base * base * base * base * base;
    return res;
}
