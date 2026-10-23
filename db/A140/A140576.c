/*
 * A140576  Numbers of the form i*9^j-1 (i=1..8, j >= 0).
 * https://oeis.org/A140576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1) % 8 + 1) * 9^((n-1) / 8) - 1
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A140576: numbers of the form i*9^j-1
   (i = 1..8, j >= 0), indexed starting at n = 1. */

seq_t a(idx_t n)
{
    /* n >= 1 */
    idx_t j = (n - 1) / 8;           /* exponent j */
    idx_t i = (n - 1) % 8 + 1;       /* multiplier i (1..8) */

    /* compute 9^j in seq_t */
    seq_t pow = (seq_t)1;
    for (idx_t p = 0; p < j; ++p)
        pow *= (seq_t)9;

    /* compute i * 9^j - 1 */
    seq_t val = pow * (seq_t)i;
    val -= (seq_t)1;

    return val;
}
