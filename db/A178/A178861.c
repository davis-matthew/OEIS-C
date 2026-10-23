/*
 * A178861  Divisors of 15120.
 * https://oeis.org/A178861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, easy, full
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A178861: divisors of 15120 in increasing order */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* comparator for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if (x < y) return -1;
    if (x > y) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    /* 15120 = 2^4 * 3^3 * 5^1 * 7^1 – 80 divisors in total */
    static bool init = false;
    static uint64_t divs[80];

    if (!init) {
        size_t idx = 0;
        for (idx_t e2 = 0; e2 <= 4; ++e2) {
            uint64_t val2 = 1;
            for (idx_t i = 0; i < e2; ++i) val2 *= 2;

            for (idx_t e3 = 0; e3 <= 3; ++e3) {
                uint64_t val3 = val2;
                for (idx_t i = 0; i < e3; ++i) val3 *= 3;

                for (idx_t e5 = 0; e5 <= 1; ++e5) {
                    uint64_t val5 = val3;
                    if (e5) val5 *= 5;

                    for (idx_t e7 = 0; e7 <= 1; ++e7) {
                        uint64_t val = val5;
                        if (e7) val *= 7;
                        divs[idx++] = val;
                    }
                }
            }
        }
        /* sort the computed divisors */
        qsort(divs, 80, sizeof(uint64_t), cmp_uint64);
        init = true;
    }

    if (n == 0 || n > 80)
        return (seq_t)0;          /* undefined for out‑of‑range indices */

    return (seq_t)divs[n - 1];
}
