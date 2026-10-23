/*
 * A040450  Continued fraction for sqrt(472).
 * https://oeis.org/A040450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients of sqrt(472) */
seq_t a(idx_t n)
{
    /* Periodic part of length 14: a(1) … a(14) */
    static const seq_t period[14] = {1, 2, 1, 1, 1, 4, 5, 4, 1, 1, 1, 2, 1, 42};
    if (n == 0)
        return 21;
    idx_t k = (n - 1) % 14;   /* Index within the period (0 … 13) */
    return period[k];
}
