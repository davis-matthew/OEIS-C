/*
 * A018635  Divisors of 768.
 * https://oeis.org/A018635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 768: a(n) is the n-th smallest divisor of 768, sorted ascending. */
/* Computation is performed by brute‑force enumeration of divisors of 768. */

#include <stdbool.h>
#include <stddef.h>

static bool ready = false;
static seq_t divisors[100];
static size_t divisor_count = 0;

seq_t a(idx_t n)
{
    if (!ready) {
        /* generate all divisors of 768 */
        const idx_t TARGET = 768;
        size_t pos = 0;
        for (idx_t i = 1; i <= TARGET; ++i) {
            if (TARGET % i == 0) {
                divisors[pos++] = (seq_t)i;
            }
        }
        divisor_count = pos;

        /* simple insertion sort (divisor_count is at most 18) */
        for (size_t i = 0; i < divisor_count; ++i) {
            for (size_t j = i + 1; j < divisor_count; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        ready = true;
    }

    if (n < 1 || n > divisor_count) {
        return (seq_t)0;   /* undefined for out-of-range indices */
    }
    return divisors[n - 1];
}
