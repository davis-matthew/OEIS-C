/*
 * A018607  Divisors of 715.
 * https://oeis.org/A018607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 715: a(n) is the nth divisor of 715 in ascending order */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The divisors of 715 = 5 * 11 * 13 are all products of subsets of
       {5, 11, 13}. There are 2^3 = 8 of them. */
    static bool initialized = false;
    static seq_t divisors[8];
    if (!initialized) {
        const seq_t primes[3] = { (seq_t)5, (seq_t)11, (seq_t)13 };
        /* Generate all 2^3 combinations */
        for (idx_t mask = 0; mask < 8; ++mask) {
            seq_t v = (seq_t)1;
            for (int i = 0; i < 3; ++i) {
                if (mask & (1U << i))
                    v *= primes[i];
            }
            divisors[mask] = v;
        }
        /* Simple bubble sort (only 8 elements) */
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 7 - i; ++j) {
                if (divisors[j] > divisors[j + 1]) {
                    seq_t tmp = divisors[j];
                    divisors[j] = divisors[j + 1];
                    divisors[j + 1] = tmp;
                }
            }
        }
        initialized = true;
    }
    if (n >= 1 && n <= 8) {
        return divisors[(size_t)(n - 1)];
    }
    return (seq_t)0;
}
