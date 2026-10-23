/*
 * A039295  Numbers whose base-6 representation has the same nonzero number of
 *          0's and 3's.
 * https://oeis.org/A039295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039295: numbers whose base‑6 representation contains an
 * equal non‑zero number of 0’s and 3’s.  The function enumerates
 * integers sequentially and keeps a cache of the qualifying values. */

#include <stdbool.h>
#include <stdlib.h>

static inline bool ok(seq_t x)
{
    unsigned cnt0 = 0, cnt3 = 0;
    if (x == 0) return false;
    while (x > 0) {
        uint8_t d = (uint8_t)(x % 6);
        if (d == 0) ++cnt0;
        else if (d == 3) ++cnt3;
        x /= 6;
    }
    return cnt0 > 0 && cnt0 == cnt3;
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache   = NULL;
    static idx_t  count   = 0;      /* number of stored terms */
    static size_t capacity = 0;     /* allocated slot count */
    static seq_t next_val = 1;      /* next integer to test */

    if (n == 0) return 0; /* safe guard for n < 1 */

    /* initialise storage if first call */
    if (cache == NULL) {
        capacity = 64;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (count < n) {
        if (ok(next_val)) {
            if (count == capacity) {
                size_t new_cap = capacity * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                capacity = new_cap;
            }
            cache[count++] = next_val;
        }
        ++next_val;
    }

    return cache[n - 1];
}
