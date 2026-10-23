/*
 * A018404  Divisors of 345.
 * https://oeis.org/A018404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 345 in ascending order (OEIS A018404)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Lazy static initialization of the divisor list */
    static bool initialized = false;
    static seq_t divisors[8];
    static size_t ndiv = 0;

    if (!initialized) {
        /* 345 = 3^1 * 5^1 * 23^1 */
        const uint64_t primes[3] = {3, 5, 23};
        const unsigned exps[3]  = {1, 1, 1};
        size_t count = 1;
        divisors[0] = (seq_t)1;

        for (size_t p = 0; p < 3; ++p) {
            size_t old_count = count;
            seq_t prime_val = (seq_t)primes[p];
            for (size_t i = 0; i < old_count; ++i) {
                seq_t base = divisors[i];
                seq_t cur = base * prime_val;
                for (unsigned e = 1; e < exps[p]; ++e) {
                    /* Not needed here because each exponent is 1 */
                }
                /* Store the new divisor */
                divisors[count++] = cur;
            }
        }
        /* Sort the list (simple insertion sort, size <= 8) */
        for (size_t i = 1; i < count; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        ndiv = count;  /* Should be 8 */
        initialized = true;
    }

    /* Return nth divisor if within bounds */
    if (n >= 1 && (idx_t)(n - 1) < ndiv) {
        return divisors[(size_t)(n - 1)];
    }
    /* Undefined sequence value for out-of-range n */
    return (seq_t)0;
}
