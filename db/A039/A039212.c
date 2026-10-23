/*
 * A039212  Numbers whose base-12 representation has the same number of 0's
 *          and 3's.
 * https://oeis.org/A039212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   searches for numbers whose base‑12 representation has equal
 *            number of 0's and 3's
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple search.  It keeps a static cache so
   repeated calls are efficient, even when the arguments are out of
   order.  The code works for all supported seq_t types (plain integer
   types or the arbitrary‑precision class). */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache of the sequence values */
static seq_t *cache = NULL;   /* array of the found terms */
static idx_t cache_len = 0;   /* number of elements stored */
static size_t cache_cap = 0;  /* capacity of the array */

static seq_t last_checked = 0;   /* last integer that was examined */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1 (offset = 1).  Return 0 for
       n == 0 (in practice this case is not used). */
    if (n == 0) return 0;

    /* Return cached value if already computed. */
    if (n <= cache_len) return cache[n-1];

    /* Allocate initial cache space if needed. */
    if (cache == NULL) {
        cache_cap = 256;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Main loop: keep searching until we have at least n values. */
    while (cache_len < n) {
        /* Move to the next integer to test. */
        seq_t val = last_checked + 1;
        last_checked = val;

        /* Count 0's and 3's in the base‑12 representation. */
        seq_t tmp = val;
        unsigned int c0 = 0, c3 = 0;
        while (tmp != 0) {
            seq_t d = tmp % 12;          /* digit (seq_t type) */
            if (d == 0) ++c0;            /* digit is 0 */
            else if (d == 3) ++c3;       /* digit is 3 */
            tmp = tmp / 12;
        }

        /* If the counts match, store the value. */
        if (c0 == c3) {
            /* Ensure the cache array has room. */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len] = val;
            ++cache_len;
        }
    }

    return cache[n-1];
}
