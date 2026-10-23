/*
 * A040541  Continued fraction for sqrt(565).
 * https://oeis.org/A040541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction coefficients for sqrt(565).
   The expansion starts with a0 = 23 and the fractional part repeats
   with period 13: 
   1, 3, 2, 1, 11, 5, 5, 11, 1, 2, 3, 1, 46. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 23;   /* a0 = floor(sqrt(565)) */

    /* Periodic part of the continued fraction (length 13). */
    static const seq_t period[13] = {1, 3, 2, 1, 11, 5, 5, 11, 1, 2, 3, 1, 46};

    return period[(n - 1) % 13];
}
