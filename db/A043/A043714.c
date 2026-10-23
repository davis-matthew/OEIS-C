/*
 * A043714  Numbers whose base-11 representation has an odd number of runs.
 * https://oeis.org/A043714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑11 representation has an odd number of runs
 *            (OEIS A043714)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Global cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static seq_t next_candidate = 1;   /* next integer to test */

/* Return the n-th term (1‑based index) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety; offset is 1 */

    /* Ensure that the cache holds at least n terms */
    if ((size_t)(n - 1) >= cache_len) {
        while (cache_len < n) {
            seq_t temp = next_candidate;
            int runs = 0;
            int prev = -1;

            /* Count runs in the base‑11 representation of next_candidate */
            while (temp > 0) {
                unsigned digit = (unsigned)(temp % 11);
                temp /= 11;
                if (runs == 0) {
                    runs = 1;
                    prev = digit;
                } else if (digit != prev) {
                    runs++;
                    prev = digit;
                }
            }

            /* If runs is odd, store the number */
            if (runs & 1) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!newp) exit(1);
                    cache = newp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_candidate;
            }

            next_candidate++;
        }
    }

    return cache[(size_t)(n - 1)];
}
