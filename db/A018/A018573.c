/*
 * A018573  Divisors of 652.
 * https://oeis.org/A018573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 652 in increasing order, for n ≥ 1.
 *            The sequence contains all positive divisors of 652, sorted in
 *            ascending order. For n larger than the number of divisors (6)
 *            the function returns 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t divisor_list[20];
static idx_t divisor_count = 0;
static bool initialized = false;

static void init_divisors(void) {
    seq_t val = 652;
    for (idx_t i = 1; ((seq_t)i * (seq_t)i) <= val; ++i) {
        /* check if i divides val */
        if (val % (seq_t)i == 0) {
            divisor_list[divisor_count++] = (seq_t)i;
            seq_t q = val / (seq_t)i;
            if (q != (seq_t)i) {
                divisor_list[divisor_count++] = q;
            }
        }
    }
    /* simple insertion sort (divisor_count is small) */
    for (idx_t i = 0; i < divisor_count; ++i) {
        for (idx_t j = i + 1; j < divisor_count; ++j) {
            if (divisor_list[i] > divisor_list[j]) {
                seq_t tmp = divisor_list[i];
                divisor_list[i] = divisor_list[j];
                divisor_list[j] = tmp;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_divisors();
    if (n == 0) return 0; /* offset is 1, so n=0 is out of range */
    if (n <= divisor_count) {
        return divisor_list[n - 1];
    }
    return 0; /* undefined outside the divisor set */
}
