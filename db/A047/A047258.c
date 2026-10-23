/*
 * A047258  Numbers that are congruent to {0, 4, 5} mod 6.
 * https://oeis.org/A047258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - 2 + ((2*n - 2) % 3) (OEIS A047258)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1, compute the value congruent to {0,4,5} mod 6 */
    seq_t n_seq = (seq_t)n;
    seq_t x = 2 * n_seq - 2;       /* 2*n - 2  (guaranteed >= 0 for n >= 1) */
    return x + (x % 3);            /* add remainder mod 3 to get final term */
}
