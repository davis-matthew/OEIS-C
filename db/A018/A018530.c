/*
 * A018530  Divisors of 580.
 * https://oeis.org/A018530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 580 sorted ascending (1‑indexed) */
#include <stdbool.h>
#include <stddef.h>

/* a(n) = the n‑th smallest divisor of 580, for n ≥ 1.
   For n > 12 (the total number of divisors) the function returns 0. */
seq_t a(idx_t n) {
    static seq_t divisors[12];
    static bool initialized = false;

    if (!initialized) {
        size_t idx = 0;
        for (int e2 = 0; e2 <= 2; ++e2) {          /* 2^e2 : 0..2 */
            seq_t p2 = ((seq_t)1 << e2);           /* 1, 2, 4 */
            for (int e5 = 0; e5 <= 1; ++e5) {      /* 5^e5 : 0..1 */
                seq_t p5 = e5 ? 5 : 1;             /* 1, 5 */
                for (int e29 = 0; e29 <= 1; ++e29) { /* 29^e29: 0..1 */
                    seq_t p29 = e29 ? 29 : 1;      /* 1, 29 */
                    divisors[idx++] = p2 * p5 * p29;
                }
            }
        }
        /* simple insertion sort for 12 elements */
        for (size_t i = 1; i < 12; ++i) {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j - 1] > key) {
                divisors[j] = divisors[j - 1];
                --j;
            }
            divisors[j] = key;
        }
        initialized = true;
    }

    if (n == 0 || n > 12)
        return (seq_t)0;          /* out of range */
    return divisors[(size_t)(n - 1)];
}
