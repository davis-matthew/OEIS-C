/*
 * A018664  Divisors of 820.
 * https://oeis.org/A018664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 820 (1-indexed)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t divisors[12];
static bool init = false;

static void init_divisors(void) {
    size_t idx = 0;
    for (unsigned e1 = 0; e1 <= 2; ++e1) {
        seq_t val1 = 1;
        for (unsigned i = 0; i < e1; ++i) val1 *= (seq_t)2;
        for (unsigned e2 = 0; e2 <= 1; ++e2) {
            seq_t val2 = val1;
            for (unsigned i = 0; i < e2; ++i) val2 *= (seq_t)5;
            for (unsigned e3 = 0; e3 <= 1; ++e3) {
                seq_t val3 = val2;
                for (unsigned i = 0; i < e3; ++i) val3 *= (seq_t)41;
                divisors[idx++] = val3;
            }
        }
    }
    /* simple selection sort (12 elements) */
    for (size_t i = 0; i < 12; ++i) {
        for (size_t j = i + 1; j < 12; ++j) {
            if (divisors[j] < divisors[i]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }
    init = true;
}

seq_t a(idx_t n) {
    if (!init) init_divisors();
    if (n < 1 || n > 12) return 0;
    return divisors[(size_t)(n - 1)];
}
