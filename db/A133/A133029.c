/*
 * A133029  Divisors of 1729, the 2nd taxicab number (also called the
 *          Hardy-Ramanujan number).
 * https://oeis.org/A133029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = the n-th divisor of 1729 in ascending order (a(1)=1)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cached divisors of 1729 */
    static bool initialized = false;
    static seq_t divisors[16];
    static idx_t divisor_count = 0;

    if (!initialized) {
        seq_t target = (seq_t)1729;

        /* Find all divisors by trial division up to sqrt(target) */
        for (seq_t i = (seq_t)1; i <= target / i; ++i) {
            if (target % i == (seq_t)0) {
                /* i is a divisor */
                divisors[divisor_count++] = i;
                /* add the complementary divisor if distinct */
                if (i != target / i) {
                    divisors[divisor_count++] = target / i;
                }
            }
        }

        /* Simple insertion sort – divisor_count is at most 8 */
        for (idx_t i = (idx_t)0; i < divisor_count; ++i) {
            for (idx_t j = i + (idx_t)1; j < divisor_count; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }

        initialized = true;
    }

    if (n == (idx_t)0) {
        /* Sequence starts at index 1; return 0 for undefined indices */
        return (seq_t)0;
    }

    if (n - (idx_t)1 < divisor_count) {
        return divisors[n - (idx_t)1];
    }

    /* For indices beyond the finite sequence, return 0 */
    return (seq_t)0;
}
