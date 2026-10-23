/*
 * A018762  Divisors of 990.
 * https://oeis.org/A018762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 990 in ascending order
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* cache of divisors computed once */
    static seq_t cache[24] = {0};
    static size_t cache_size = 0;
    if (cache_size == 0) {
        seq_t m = 990;
        size_t cnt = 0;
        for (seq_t d = 1; d <= m; ++d) {
            if (m % d == 0) {
                cache[cnt++] = d;
            }
        }
        cache_size = cnt; /* should be 24 */
    }
    if (n == 0) return 0; /* out of bounds, return 0 */
    if ((idx_t)(n - 1) < cache_size) {
        return cache[(size_t)(n - 1)];
    }
    /* for n > number of divisors, sequence is undefined; return 0 */
    return 0;
}
