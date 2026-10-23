/*
 * A162819  Positive numbers n such that 48*n/(48+n) are integers.
 * https://oeis.org/A162819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   48*n/(48+n) is integer <=> n = d-48 where d|2304 and d>48
 * Range:     u8 none, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t cache[27];
    static size_t cached_len = 0;
    static bool initialized = false;

    if (!initialized) {
        const idx_t LIMIT = 2304;
        seq_t tmp[27];
        size_t tmpcnt = 0;

        for (idx_t i = 1; i * i <= LIMIT; ++i) {
            if (LIMIT % i == 0) {
                if (i > 48)
                    tmp[tmpcnt++] = (seq_t)i;
                idx_t j = LIMIT / i;
                if (j != i && j > 48)
                    tmp[tmpcnt++] = (seq_t)j;
            }
        }

        /* sort the divisors in ascending order */
        for (size_t p = 1; p < tmpcnt; ++p) {
            seq_t key = tmp[p];
            size_t q = p;
            while (q > 0 && tmp[q - 1] > key) {
                tmp[q] = tmp[q - 1];
                --q;
            }
            tmp[q] = key;
        }

        for (size_t i = 0; i < tmpcnt; ++i)
            cache[i] = tmp[i] - (seq_t)48;   /* n = d - 48 */

        cached_len = tmpcnt;
        initialized = true;
    }

    if (n == 0 || n > cached_len)
        return (seq_t)0;

    return cache[n - 1];
}
