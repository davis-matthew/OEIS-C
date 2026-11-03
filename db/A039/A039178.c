/*
 * A039178  Numbers whose base-11 representation has the same number of 2's
 *          and 7's.
 * https://oeis.org/A039178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th non‑negative integer whose base‑11
 *            representation contains an equal number of digits 2 and 7. (OEIS
 *            A039178)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: test whether num has equal count of 2's and 7's in base 11 */
static int has_equal_27(seq_t num)
{
    int cnt2 = 0, cnt7 = 0;
    seq_t x = num;
    while (x > (seq_t)0) {
        seq_t digit = x % (seq_t)11;
        if (digit == (seq_t)2) cnt2++;
        else if (digit == (seq_t)7) cnt7++;
        x = x / (seq_t)11;
    }
    return cnt2 == cnt7;
}

static seq_t *cache = NULL;      /* store found terms */
static size_t cache_size = 0;    /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity of cache */
static seq_t next_candidate = (seq_t)0;  /* next integer to test */

/* Ensure the cache contains at least n terms (n >= 1) */
static void ensure_cache(idx_t n)
{
    while (cache_size < (size_t)n) {
        /* search for the next qualifying number */
        if (has_equal_27(next_candidate)) {
            /* grow cache if necessary */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
        next_candidate++;          /* advance to the next integer for testing */
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;  /* undefined for n < 1, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
