/*
 * A043467  Numbers having three 3's in base 9.
 * https://oeis.org/A043467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): nth positive integer whose base‑9 representation contains exactly three digits equal to 3. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 */

    static idx_t max_cached_n = 0; /* number of terms already cached */
    static seq_t *cache = NULL;    /* cache array */
    static size_t cache_cap = 0;   /* current capacity of the cache */
    static idx_t last_checked = 0; /* last decimal number examined */

    while (max_cached_n < n) {
        /* examine next candidate integer */
        last_checked++;

        seq_t x = (seq_t)last_checked;
        idx_t cnt3 = 0;
        while (x > 0) {
            if ((x % (seq_t)9) == 3) {
                cnt3++;
                if (cnt3 > 3) break;  /* early exit if too many 3's */
            }
            x /= (seq_t)9;
        }

        if (cnt3 == 3) {
            /* store the found term in the cache */
            if (max_cached_n >= cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[max_cached_n++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
