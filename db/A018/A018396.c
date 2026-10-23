/*
 * A018396  Divisors of 330.
 * https://oeis.org/A018396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth smallest divisor of 330 (1 ≤ n ≤ 16)
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t cache[16];
    static int init = 0;
    if (!init) {
        const seq_t pr[4] = {2, 3, 5, 11};
        size_t cnt = 0;
        for (size_t mask = 0; mask < (1ull << 4); ++mask) {
            seq_t prod = 1;
            for (size_t bit = 0; bit < 4; ++bit) {
                if (mask & (1ull << bit))
                    prod *= pr[bit];
            }
            cache[cnt++] = prod;
        }
        /* insertion sort */
        for (size_t i = 1; i < cnt; ++i) {
            seq_t key = cache[i];
            size_t j = i;
            while (j > 0 && key < cache[j - 1]) {
                cache[j] = cache[j - 1];
                --j;
            }
            cache[j] = key;
        }
        init = 1;
    }
    if (n == 0) return 0;
    size_t idx = (size_t)(n - 1);
    if (idx >= 16) return 0;
    return cache[idx];
}
