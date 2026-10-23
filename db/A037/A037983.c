/*
 * A037983  Maximal base 5 run length is 4.
 * https://oeis.org/A037983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th positive integer whose base‑5 representation contains a
   maximal run of consecutive identical digits of length exactly 4. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* static storage for memoised values */
static seq_t *cache = NULL;          /* array of cached sequence terms      */
static size_t cache_cap = 0;         /* current capacity of cache array    */
static idx_t cache_len  = 0;         /* number of terms cached              */
static seq_t next_candidate = 1;      /* next integer to test in scan       */

/* ensure that cache contains at least n terms (n >= 1) */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    /* initialise cache if necessary */
    if (cache == NULL) {
        cache_cap = 64;
        cache = seq_alloc_new_array(cache_cap);
        cache_len = 0;
    }

    while (cache_len < n) {
        /* one candidate */
        seq_t m = next_candidate;
        ++next_candidate;

        /* compute longest run of equal digits in base 5 */
        seq_t temp = m;
        seq_t max_run = 0;
        seq_t cur_run = 0;
        seq_t prev_digit = 5;   /* sentinel value larger than any base‑5 digit */

        while (temp > 0) {
            seq_t digit = temp % 5;
            temp /= 5;
            if (digit == prev_digit) {
                ++cur_run;
            } else {
                prev_digit = digit;
                cur_run = 1;
            }
            if (cur_run > max_run)
                max_run = cur_run;
            if (max_run > 4)          /* nothing longer than 4 is allowed   */
                break;
        }

        if (max_run == 4) {
            /* store term, grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
                seq_t *new_cache = seq_alloc_new_array(new_cap);
                for (size_t i = 0; i < cache_cap; ++i)
                    new_cache[i] = cache[i];
                if (cache != NULL)
                    seq_free_array(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
    }
}

seq_t a(idx_t n)
{
    /* fopenm sequence indices start at 1 */
    if (n < 1)
        return 0;                      /* undefined for n < 1, but return 0 */

    ensure_cache(n);
    return cache[n - 1];
}
