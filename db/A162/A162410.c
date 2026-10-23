/*
 * A162410  Numbers n such that 10*n + 3 and 10*n + 7 are prime.
 * https://oeis.org/A162410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer n such that 10*n+3 and 10*n+7 are prime
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; behaviour below offset is irrelevant */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_check = 0;         /* next n to test */

    /* Generate terms until we have at least n values */
    while (cache_len < n) {
        seq_t p1 = (seq_t)(10 * (seq_t)next_check + (seq_t)3);
        seq_t p2 = (seq_t)(10 * (seq_t)next_check + (seq_t)7);

        if (isPrimeGeneric(p1) && isPrimeGeneric(p2)) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();        /* allocation failed */
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_check;
        }
        ++next_check;
    }

    return cache[n - 1];
}
