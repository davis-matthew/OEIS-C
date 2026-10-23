/*
 * A163354  Divisors of 13237.
 * https://oeis.org/A163354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 13237 in increasing order (finite
 *            sequence)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* number of divisors of 13237 = 2^3 = 8 */
    static seq_t divisors[8];
    static int initialized = 0;

    if (!initialized) {
        /* prime factors of 13237 */
        seq_t primes[3] = {(seq_t)7, (seq_t)31, (seq_t)61};

        /* generate all 2^3 divisors by bitmask */
        size_t mask;
        for (mask = 0; mask < (size_t)8; ++mask) {
            seq_t val = (seq_t)1;
            if (mask & 1u) val *= primes[0];
            if (mask & 2u) val *= primes[1];
            if (mask & 4u) val *= primes[2];
            divisors[mask] = val;
        }

        /* sort ascending (simple bubble sort, works for 8 elements) */
        int i, j;
        for (i = 0; i < 7; ++i) {
            for (j = 0; j < 7 - i; ++j) {
                if (divisors[j] > divisors[j + 1]) {
                    seq_t tmp = divisors[j];
                    divisors[j] = divisors[j + 1];
                    divisors[j + 1] = tmp;
                }
            }
        }

        initialized = 1;
    }

    if (n == (idx_t)0)
        return (seq_t)0;
    if (n <= (idx_t)8)
        return divisors[(size_t)(n - 1)];
    return (seq_t)0;
}
