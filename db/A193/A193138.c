/*
 * A193138  Number of square satins of order n.
 * https://oeis.org/A193138
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of square satins of order n Defined for n ≥ 3 by
 *            the prime factorisation n = 2^a * ∏ p_i^b_i * ∏ q_j^c_j where
 *            p_i ≡ 1 (mod 4), q_j ≡ 3 (mod 4). If a = 0 or 1 and no q_j’s
 *            occur, then a(n) = 2^(t-1), where t is the number of distinct
 *            p_i’s. Otherwise a(n) = 0
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;            /* not needed for the official range */

    uint64_t m = (uint64_t)n;
    uint64_t aexp = 0;

    /* factor powers of two */
    while ((m & 1ULL) == 0) {
        aexp++;
        m >>= 1;
    }
    if (aexp > 1) return 0;         /* exponent of 2 too large */

    uint64_t distinct1mod4 = 0;   /* count t */

    /* factor odd primes */
    uint64_t i = 3;
    while (i <= m / i) {
        if (m % i == 0) {
            bool is1mod4 = (i % 4ULL) == 1ULL;
            if (!is1mod4) return 0;   /* prime ≡ 3 (mod 4) found */

            /* divide out the entire prime power */
            do {
                m /= i;
            } while (m % i == 0);
            distinct1mod4++;          /* count distinct prime once */
        }
        i += 2;
    }

    if (m > 1) {                     /* remaining prime factor */
        if ((m % 4ULL) == 3ULL) return 0;
        if ((m % 4ULL) == 1ULL) distinct1mod4++;
    }

    if (distinct1mod4 == 0) return 0;  /* no qualifying primes */

    /* compute 2^(t-1) */
    seq_t result = 1;
    for (uint64_t k = 0; k < distinct1mod4 - 1; ++k)
        result <<= 1;

    return result;
}
