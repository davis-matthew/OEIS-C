/*
 * A165305  a(n) is the smallest number not yet in the sequence such that the
 *          concatenation of all terms yields a periodic stream of digits 1,
 *          2, 3, ..., 7 (repeat from 1).
 * https://oeis.org/A165305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   compose_num, digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A165305: a(n) is the smallest number not yet in the sequence such that the
   concatenation of all terms yields the periodic digit stream 1,2,3,4,5,6,7,1,… */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_len.h"
#include "../utils/compose_num.h"

/* cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* state of the construction */
static idx_t total_digits = 0;          /* digits already emitted */
static idx_t cur_len[8] = {0};          /* smallest unused length for start digit 1..7 */

/* increase cache capacity to hold at least `n` terms */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;                               /* already enough */

    /* enlarge storage if needed */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr)
            abort();                          /* allocation failed */
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* initialise state on the first call */
    if (cache_len == 0) {
        for (idx_t d = 1U; d <= 7U; ++d)
            cur_len[d] = 1U;
        total_digits = 0;
    }

    /* generate terms until we reach `n` */
    for (size_t idx = cache_len; idx < (size_t)n; ++idx) {
        idx_t d = (total_digits % 7U) + 1U;            /* next required digit */
        seq_t val = compose_num(d, cur_len[d]);        /* term value          */
        cache[idx] = val;

        total_digits += digit_len(val);                /* update digit count   */
        ++cur_len[d];                                 /* next length for d    */
    }
    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                            /* undefined for n==0 */
    ensure_cache(n);
    return cache[n - 1];
}
