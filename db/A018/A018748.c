/*
 * A018748  Divisors of 966.
 * https://oeis.org/A018748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest positive divisor of 966
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If n > 16 or n == 0, the function returns 0. */

seq_t a(idx_t n)
{
    static seq_t divisors[16];
    static int initialized = 0;

    if (!initialized) {
        /* Prime factorization of 966: 2 * 3 * 7 * 23 */
        const seq_t primes[4] = {2, 3, 7, 23};
        /* Generate all 2^4 = 16 divisors by subset multiplication */
        for (idx_t mask = 0; mask < 16; ++mask) {
            seq_t prod = 1;
            for (idx_t i = 0; i < 4; ++i) {
                if (mask & (1ULL << i))
                    prod *= primes[i];
            }
            divisors[mask] = prod;
        }
        /* Sort the divisors in ascending order (simple O(n²) sort). */
        for (idx_t i = 0; i < 15; ++i) {
            for (idx_t j = i + 1; j < 16; ++j) {
                if (divisors[i] > divisors[j]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        initialized = 1;
    }

    if (n == 0)
        return 0;

    idx_t idx = n - 1;
    if (idx >= 16)
        return 0;

    return divisors[idx];
}
