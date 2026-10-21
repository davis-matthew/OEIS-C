/*
 * A230149  The number of multinomial coefficients over partitions with value
 *          equal to 5.
 * https://oeis.org/A230149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/4) + floor((n-1)/5) - floor(n/5)
 * Range:     u8 n <= 269, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 0, a(5) = 0, a(6) = 1, ... */

seq_t a(idx_t n)
{
    /* Safety: if n is 0 (outside the offset) return 0 */
    if (n == 0) return 0;

    idx_t n_minus1 = n - 1;

    seq_t t1 = ((seq_t)n_minus1) / 4;   /* floor((n-1)/4) */
    seq_t t2 = ((seq_t)n_minus1) / 5;   /* floor((n-1)/5) */
    seq_t t3 = ((seq_t)n)        / 5;   /* floor(n/5) */

    return t1 + t2 - t3;
}
