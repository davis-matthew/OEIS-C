/*
 * A018508  Divisors of 539.
 * https://oeis.org/A018508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 539 in increasing order */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t divs[6];
    if (!initialized) {
        idx_t target = 539;
        size_t count = 0;
        for (idx_t i = 1; i * i <= target; ++i) {
            if (target % i == 0) {
                divs[count++] = (seq_t)i;
                if (i != target / i) {
                    divs[count++] = (seq_t)(target / i);
                }
            }
        }
        // sort ascending
        for (size_t i = 1; i < 6; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        initialized = 1;
    }
    if (n >= 1 && n <= 6) {
        return divs[n - 1];
    }
    return 0;
}
