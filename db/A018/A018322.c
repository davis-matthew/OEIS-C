/*
 * A018322  Divisors of 182.
 * https://oeis.org/A018322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th smallest divisor of 182 (0 for n>8) */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    static seq_t divs[32];
    static size_t divcnt = 0;
    static bool init = false;
    if (!init) {
        uint64_t target = 182;
        for (uint64_t i = 1; i * i <= target; ++i) {
            if (target % i == 0) {
                divs[divcnt++] = (seq_t)i;
                if (i != target / i)
                    divs[divcnt++] = (seq_t)(target / i);
            }
        }
        /* simple insertion sort (divcnt <= 8) */
        for (size_t i = 1; i < divcnt; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        init = true;
    }
    if (n <= divcnt)
        return divs[n - 1];
    return 0;
}
