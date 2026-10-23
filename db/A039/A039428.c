/*
 * A039428  Numbers whose base-10 representation has the same nonzero number
 *          of 5's and 6's.
 * https://oeis.org/A039428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose decimal representation *
 *            contains the same nonzero number of 5’s and 6’s
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Ensure the cache has space for at least 'needed' elements. */
static void ensure_capacity(idx_t needed, idx_t *cap, seq_t **ptr)
{
    if (needed <= *cap) return;
    idx_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*ptr,
                                  (size_t)new_cap * sizeof(seq_t));
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for offset 1, but harmless */

    static seq_t *cache   = NULL;
    static idx_t cap      = 0;
    static idx_t len      = 0;   /* number of values stored in cache   */
    static uint64_t next_val = 1; /* next integer to test               */

    while (len < n) {
        uint64_t v = next_val;
        unsigned count5 = 0, count6 = 0;

        if (v > 0) {
            uint64_t temp = v;
            while (temp > 0) {
                uint64_t d = temp % 10;
                if (d == 5)   count5++;
                else if (d == 6) count6++;
                temp /= 10;
            }
            if (count5 == count6 && count5 > 0) {
                ensure_capacity(len + 1, &cap, &cache);
                cache[len] = (seq_t)v;
                len++;
            }
        }
        next_val++;
    }

    return cache[n - 1];
}
