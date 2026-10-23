/*
 * A018559  Divisors of 630.
 * https://oeis.org/A018559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 630 in ascending order */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Sequence of divisors of 630, stored in ascending order.
   The initializer is performed lazily in init_divisors(). */
static seq_t divisors[24];
static bool initialized = false;

/* Initialize the divisor table so that divisors[] is sorted.
   The table is small (24 elements) so insertion sort is sufficient
   and does not require any external utilities. */
static void init_divisors(void) {
    if (initialized) {
        return;
    }

    /* Pre‑compute the powers of each prime factor to avoid repeated
       multiplications inside the loops. */
    seq_t pow2[2]  = { (seq_t)1, (seq_t)2 };
    seq_t pow3[3]  = { (seq_t)1, (seq_t)3, (seq_t)9 };
    seq_t pow5[2]  = { (seq_t)1, (seq_t)5 };
    seq_t pow7[2]  = { (seq_t)1, (seq_t)7 };

    /* Generate all 2 × 3 × 2 × 2 = 24 divisors of 630. */
    idx_t pos = 0;
    for (idx_t e2 = 0; e2 < 2; ++e2) {
        for (idx_t e3 = 0; e3 < 3; ++e3) {
            for (idx_t e5 = 0; e5 < 2; ++e5) {
                for (idx_t e7 = 0; e7 < 2; ++e7) {
                    seq_t val = pow2[e2];
                    val *= pow3[e3];
                    val *= pow5[e5];
                    val *= pow7[e7];
                    divisors[pos++] = val;
                }
            }
        }
    }

    /* Insertion sort the array in ascending order.  This works for
       both primitive integer types (uint8_t … uint64_t) and for the
       arbitrary‑precision class provided by the build (whose operators
       <, >, = are overloaded). */
    for (idx_t i = 1; i < 24; ++i) {
        seq_t key = divisors[i];
        idx_t j = i;
        while (j > 0 && key < divisors[j - 1]) {
            divisors[j] = divisors[j - 1];
            --j;
        }
        divisors[j] = key;
    }

    initialized = true;
}

/* External linkage function required by the test harness. */
seq_t a(idx_t n) {
    init_divisors();
    if (n == 0 || n > 24) {
        return (seq_t)0;
    }
    return divisors[n - 1];
}
