/*
 * A220096  a(1)=0, n-1 if n is prime, else largest proper divisor of n.
 * https://oeis.org/A220096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n=1, n-1 when n is prime, otherwise largest proper
 *            divisor of n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* Find the smallest divisor > 1 using trial division. */
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            /* Composite: largest proper divisor is n/d. */
            return (seq_t)(n / d);
        }
    }

    /* No divisor found: n is prime. */
    return (seq_t)(n - 1);
}
