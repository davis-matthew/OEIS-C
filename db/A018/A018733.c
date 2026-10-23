/*
 * A018733  Divisors of 940.
 * https://oeis.org/A018733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 940 (sorted ascending)
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* divisors of 940 = 2^2 * 5^1 * 47^1 -> 12 divisors */
    static uint64_t divs[12];
    static int initialized = 0;

    if (!initialized) {
        /* generate all divisors */
        int pos = 0;
        for (int e2 = 0; e2 <= 2; ++e2) {
            for (int e5 = 0; e5 <= 1; ++e5) {
                for (int e47 = 0; e47 <= 1; ++e47) {
                    uint64_t val = ((uint64_t)1 << e2) *
                                   (e5 ? 5ULL : 1ULL) *
                                   (e47 ? 47ULL : 1ULL);
                    divs[pos++] = val;
                }
            }
        }
        /* sort ascending (simple insertion sort, only 12 elements) */
        for (int i = 1; i < 12; ++i) {
            uint64_t key = divs[i];
            int j = i - 1;
            while (j >= 0 && divs[j] > key) {
                divs[j + 1] = divs[j];
                --j;
            }
            divs[j + 1] = key;
        }
        initialized = 1;
    }

    if (n >= 1 && n <= 12) {
        return (seq_t)divs[(size_t)(n - 1)];
    }
    /* undefined for n > 12 */
    return (seq_t)0;
}
