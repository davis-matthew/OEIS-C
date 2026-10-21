/*
 * A238525  n modulo sopfr(n), where sopfr(n) is the sum of the prime factors
 *          of n, with multiplicity.
 * https://oeis.org/A238525
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod sopfr(n)
 * Range:     u8 n <= 524, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sopfr(n) is the sum of prime factors of n, counted with multiplicity. */
seq_t a(idx_t n)
{
    /* Offset is 2; behaviour for n < 2 is irrelevant. */
    if (n < 2)
        return 0;

    idx_t m = n;
    seq_t sum = 0;

    /* Handle factor 2 separately (fast). */
    while ((m & 1ULL) == 0) {
        sum += 2;
        m >>= 1;
    }

    /* Handle odd prime factors. */
    for (idx_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) {
            sum += (seq_t)p;
            m /= p;
        }
    }

    /* If any prime factor larger than sqrt(n) remains. */
    if (m > 1)
        sum += (seq_t)m;

    /* Return n modulo the sum of its prime factors. */
    return ((seq_t)n) % sum;
}
