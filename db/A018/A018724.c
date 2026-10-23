/*
 * A018724  Divisors of 925.
 * https://oeis.org/A018724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 925 in increasing order
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t divisors[6];
static int initialized = 0;

static void init_divisors(void) {
    seq_t powers5[3];
    powers5[0] = (seq_t)1;
    powers5[1] = powers5[0] * (seq_t)5;
    powers5[2] = powers5[1] * (seq_t)5;

    seq_t powers37[2];
    powers37[0] = (seq_t)1;
    powers37[1] = powers37[0] * (seq_t)37;

    size_t idx = 0;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            divisors[idx++] = powers5[i] * powers37[j];
        }
    }

    /* simple insertion sort to order divisors ascending */
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = i + 1; j < 6; ++j) {
            if (divisors[j] < divisors[i]) {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = tmp;
            }
        }
    }
}

seq_t a(idx_t n) {
    if (!initialized) {
        init_divisors();
        initialized = 1;
    }
    if (n == 0) return (seq_t)0;
    size_t k = (size_t)(n - 1);
    if (k < 6) return divisors[k];
    return (seq_t)0;
}
