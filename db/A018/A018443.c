/*
 * A018443  Divisors of 418.
 * https://oeis.org/A018443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n‑th divisor of 418
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static bool init_done = false;
static seq_t divisors[8];
static size_t d_count = 0;

static void init_divisors(void) {
    if (init_done) return;
    const idx_t num = 418;
    seq_t temp[16];
    size_t temp_count = 0;
    for (idx_t i = 1; i * i <= num; ++i) {
        if (num % i == 0) {
            temp[temp_count++] = (seq_t)i;
            if (i != num / i)
                temp[temp_count++] = (seq_t)(num / i);
        }
    }
    /* simple sort of temp[0 .. temp_count-1] */
    for (size_t i = 0; i < temp_count; ++i) {
        for (size_t j = i + 1; j < temp_count; ++j) {
            if (temp[j] < temp[i]) {
                seq_t t = temp[i];
                temp[i] = temp[j];
                temp[j] = t;
            }
        }
    }
    for (size_t i = 0; i < temp_count && i < 8; ++i) {
        divisors[i] = temp[i];
    }
    d_count = temp_count;
    init_done = true;
}

seq_t a(idx_t n) {
    init_divisors();
    if (n == 0) return 0;
    if (n <= d_count) {
        return divisors[n - 1];
    }
    return 0;
}
