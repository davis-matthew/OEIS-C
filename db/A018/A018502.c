/*
 * A018502  Divisors of 528.
 * https://oeis.org/A018502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th smallest divisor of 528 (sorted ascending) */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t divs[20];
    static size_t cnt = 0;
    static bool init = false;

    if (!init) {
        const unsigned long long e2[5] = {1ULL, 2ULL, 4ULL, 8ULL, 16ULL};
        const unsigned long long e3[2] = {1ULL, 3ULL};
        const unsigned long long e11[2] = {1ULL, 11ULL};

        size_t idx = 0;
        for (size_t i = 0; i < 5; ++i)
            for (size_t j = 0; j < 2; ++j)
                for (size_t k = 0; k < 2; ++k)
                    divs[idx++] = (seq_t)(e2[i] * e3[j] * e11[k]);

        cnt = idx;

        /* insertion sort the 20 elements */
        for (size_t i = 1; i < cnt; ++i) {
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

    if (n < cnt)
        return divs[(size_t)n];

    return (seq_t)0;
}
