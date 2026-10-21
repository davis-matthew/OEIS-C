/*
 * A204877  Continued fraction expansion of 3*tanh(1/3).
 * https://oeis.org/A204877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 0, n = 0; a(n) = (5 + 4*(-1)^n) * (2*n - 1), n >= 1
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* When n is even: (5+4*1) = 9
       When n is odd:  (5+4*(-1)) = 1   */
    seq_t factor = (n & 1) ? (seq_t)1 : (seq_t)9;

    /* Compute (2*n - 1) safely in seq_t */
    seq_t two_n_minus_one = (seq_t)(2 * (seq_t)n - 1);

    return factor * two_n_minus_one;
}
