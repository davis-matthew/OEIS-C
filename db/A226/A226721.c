/*
 * A226721  Position of 2^n in the joint ranking of all the numbers 2^j for
 *          j>=0 and 5^k for k>=1; complement of A123384.
 * https://oeis.org/A226721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n * (1 + log_5(2)))
 * Range:     u8 n <= 178, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = n + 1 + floor(n * log_5(2))          */
/* Using a high‑precision rational approximation of log_5(2) = 4.306765581e-1   */

/* In this implementation we use the rational
 *   log_5(2) ≈ 4306765581 / 10^10
 * which is accurate to better than 10^(-10).  For n <= 10^4
 * the error in n*log_5(2) is less than 10^(-6) and therefore
 * the floor is always correct.
 */

seq_t a(idx_t n)
{
    /* If n is 0 (outside the sequence offset) return 0.
     * The behaviour for n < 1 is irrelevant for the test harness. */
    if (n == 0)
        return 0;

    /* Numerator and denominator of the rational approximation of log_5(2).  */
    static const unsigned long long P = 4306765581ULL;   /* 0.4306765581 * 10^10 */
    static const unsigned long long SCALE = 10000000000ULL; /* 10^10 */

    /* Compute floor(n * P / SCALE) using 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 prod = (unsigned __int128)n * P;
    unsigned __int128 q    = prod / SCALE;   /* floor(n * log_5(2)) */

    /* Build the final result: n + 1 + q.  All operations are done in seq_t. */
    seq_t result = (seq_t)n + (seq_t)1 + (seq_t)q;
    return result;
}
