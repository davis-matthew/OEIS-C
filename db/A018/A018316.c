/*
 * A018316  Divisors of 171.
 * https://oeis.org/A018316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 171, sorted ascending */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static const uint64_t TARGET = 171ull;
    static seq_t divs[12];
    static size_t div_count = 0;
    static int initialized = 0;

    if (!initialized) {
        size_t count = 0;
        for (uint64_t i = 1; i * i <= TARGET; ++i) {
            if (TARGET % i == 0) {
                divs[count++] = (seq_t)i;
                uint64_t q = TARGET / i;
                if (q != i)
                    divs[count++] = (seq_t)q;
            }
        }
        /* simple insertion sort for the small number of elements */
        for (size_t i = 0; i < count; ++i) {
            for (size_t j = i + 1; j < count; ++j) {
                if (divs[j] < divs[i]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
        div_count = count;
        initialized = 1;
    }

    if (n > 0 && n <= div_count)
        return divs[n - 1];
    return (seq_t)0;
}
