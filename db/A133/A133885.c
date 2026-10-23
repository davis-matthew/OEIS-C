/*
 * A133885  Binomial(n+5,n) mod 5^2.
 * https://oeis.org/A133885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5) mod 25 The sequence is periodic of period
 *            125, so we reduce n modulo 125 before computing the binomial
 *            coefficient
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce index modulo the period */
    idx_t r = n % 125;

    /* Compute binomial(r+5,5) exactly using 128-bit arithmetic */
    idx_t N = r + 5;          // N = n + 5 (mod 125)
    unsigned __int128 res = 1; // intermediate accumulator

    for (idx_t i = 1; i <= 5; ++i) {
        /* Multiply by the next factor in the numerator
           and divide by i to stay exact. */
        res = res * (N - (5 - i));
        res /= i;            // exact division
    }

    /* Take the result modulo 25 and cast back to seq_t */
    return (seq_t)(res % 25);
}
