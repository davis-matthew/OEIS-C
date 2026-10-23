/*
 * A039537  Numbers whose base-12 representation has the same nonzero number
 *          of 4's and 11's.
 * https://oeis.org/A039537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force digit counting in base‑12:
   keep the numbers whose non‑zero counts of digits 4 and 11 are equal. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t next_start = 0;   /* next integer to test */

/* grow the cache as needed and fill it with additional terms */
static void ensure_cache(idx_t n)
{
    /* allocate or grow the cache if necessary */
    if (cache_cap < n) {
        size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate new valid numbers until we have n terms */
    seq_t val = next_start;
    while (cache_len < n) {
        seq_t tmp = val;
        int cnt4  = 0;
        int cnt11 = 0;

        /* count base‑12 digits 4 and 11 */
        while (tmp != 0) {
            int d = (int)(tmp % (seq_t)12);
            if (d == 4)      cnt4++;
            else if (d == 11) cnt11++;
            tmp /= (seq_t)12;
        }

        if (cnt4 > 0 && cnt4 == cnt11) {
            cache[cache_len++] = val;
        }
        ++val;
    }
    next_start = val;
}

/* Return the n‑th term (1‑based).  Undefined for n = 0; return 0 in that case. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[n - 1];
}
