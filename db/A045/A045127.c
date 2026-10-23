/*
 * A045127  Numbers whose base-4 representation contains exactly three 1's and
 *          three 3's.
 * https://oeis.org/A045127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045127: Numbers whose base‑4 representation contains exactly
   three 1's and three 3's. */

#include <stdlib.h>

/* Helper that enlarges the cache array if necessary. */
static void
ensure_capacity(size_t needed, seq_t **cache, size_t *capacity)
{
    if (needed <= *capacity)
        return;
    size_t new_cap = *capacity ? *capacity * 2 : 128;
    while (new_cap < needed)
        new_cap <<= 1;
    *cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    *capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* Static state for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_len   = 0;   /* number of terms stored */
    static size_t cache_cap   = 0;   /* current capacity of cache    */
    static seq_t last_value   = 0;   /* last term found (for continuation) */

    size_t target = (size_t)n;      /* requested index (1‑based) */

    /* Generate terms until we have at least 'target' terms. */
    while (cache_len < target) {
        seq_t x = last_value + 1;   /* start searching after last term */

        for (;;) {
            seq_t   val  = x;
            uint32_t c1 = 0, c3 = 0;

            /* Count base‑4 digits, stop early if counts exceed 3. */
            while (val != 0 && c1 <= 3 && c3 <= 3) {
                seq_t d = val & ((seq_t)3);   /* digit in base‑4 */
                if (d == (seq_t)1)
                    ++c1;
                else if (d == (seq_t)3)
                    ++c3;
                val >>= 2;
            }

            if (c1 == 3 && c3 == 3) {
                /* Accept this number.  Store it and prepare for next. */
                ensure_capacity(cache_len + 1, &cache, &cache_cap);
                cache[cache_len++] = x;
                last_value = x;
                break;
            }
            ++x;
        }
    }

    return cache[n - 1];   /* n is 1‑based */
}
