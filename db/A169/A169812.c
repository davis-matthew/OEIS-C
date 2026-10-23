/*
 * A169812  a(n) = n XOR d(n) (cf. A000005).
 * https://oeis.org/A169812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n XOR d(n), where d(n) is the number of divisors of n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the number of divisors of n using trial division with early exit */
seq_t a(idx_t n)
{
    /* Count number of divisors (d(n)) */
    idx_t temp = n;
    idx_t count = 1;

    /* Factorization by trial division */
    for (idx_t i = 2; i <= temp / i; ++i) {
        if (temp % i == 0) {
            idx_t exp = 0;
            while (temp % i == 0) {
                temp /= i;
                ++exp;
            }
            count *= (exp + 1);
        }
    }

    /* Remaining prime factor larger than sqrt(n) */
    if (temp > 1) {
        count *= 2;
    }

    /* Return n XOR d(n) */
    return ((seq_t)n) ^ ((seq_t)count);
}
