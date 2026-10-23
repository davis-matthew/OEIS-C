/*
 * A045104  Numbers whose base-4 representation contains exactly three 1's and
 *          four 2's.
 * https://oeis.org/A045104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer whose base‑4 representation contains
 *            exactly three 1's and four 2's
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* stored values */
static size_t cache_size = 0;    /* number of stored values */
static size_t cache_capacity = 0;
static seq_t next_value = 0;     /* next integer to test */

/* check if value has exactly 3 ones and 4 twos in base‑4 */
static int matches(seq_t val) {
    int c1 = 0, c2 = 0;
    seq_t v = val;
    if (v == 0) return 0;
    while (v) {
        int digit = (int)(v % 4);
        if (digit == 1) ++c1;
        else if (digit == 2) ++c2;
        if (c1 > 3 || c2 > 4) return 0;  /* early exit */
        v = v / 4;
    }
    return (c1 == 3 && c2 == 4);
}

seq_t a(idx_t n) {
    /* Ensure we have at least n terms cached */
    while (cache_size < n) {
        seq_t val = next_value;
        ++next_value;

        if (matches(val)) {
            if (cache_size == cache_capacity) {
                /* grow cache */
                size_t new_capacity = cache_capacity == 0 ? 128 : cache_capacity * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
                if (!new_cache) abort();  /* allocation failure */
                cache = new_cache;
                cache_capacity = new_capacity;
            }
            cache[cache_size++] = val;
        }
    }
    return cache[n - 1];
}
