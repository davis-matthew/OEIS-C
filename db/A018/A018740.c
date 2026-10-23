/*
 * A018740  Divisors of 952.
 * https://oeis.org/A018740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018740: a(n) = the nth divisor of 952, in increasing order.
   952 = 2^3 × 7 × 17   → 16 divisors in total.  a(1)=1, a(16)=952.  */
#include <stdlib.h>
#include <stdbool.h>

#define DIVISOR_COUNT 16

static seq_t divisors[DIVISOR_COUNT];
static bool initialized = false;

/* Return the n‑th divisor of 952 (1‑based).  For n < 1 or n > 16, return 0. */
seq_t a(idx_t n)
{
    if (!initialized) {
        size_t pos = 0;
        for (int e2 = 0; e2 <= 3; ++e2) {
            seq_t p2 = 1;
            for (int i = 0; i < e2; ++i) p2 *= 2;
            for (int e7 = 0; e7 <= 1; ++e7) {
                seq_t p7 = p2 * (e7 ? 7 : 1);
                for (int e17 = 0; e17 <= 1; ++e17) {
                    seq_t val = p7 * (e17 ? 17 : 1);
                    divisors[pos++] = val;
                }
            }
        }
        /* Simple insertion sort – only 16 elements, so O(1). */
        for (size_t i = 1; i < DIVISOR_COUNT; ++i) {
            for (size_t j = 0; j < i; ++j) {
                if (divisors[j] > divisors[i]) {
                    seq_t tmp = divisors[j];
                    divisors[j] = divisors[i];
                    divisors[i] = tmp;
                }
            }
        }
        initialized = true;
    }
    if (n < 1 || n > DIVISOR_COUNT) return 0;
    return divisors[n - 1];
}
