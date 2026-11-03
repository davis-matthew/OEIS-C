/*
 * A039184  Numbers whose base-11 representation has the same number of 3's
 *          and 6's.
 * https://oeis.org/A039184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute‑force search of natural numbers and base‑11 digit counting */
#include <stdlib.h>
#include "../utils/mem.h"

/* return 1 if v has the same number of 3’s and 6’s in base‑11, else 0 */
static int has_equal_36(seq_t v)
{
    unsigned int c3 = 0, c6 = 0;
    seq_t t = v;
    if (t == 0)         /* 0 has zero 3's and zero 6's → equal */
        return 1;
    while (t != 0) {
        int d = (int)(t % 11);
        if (d == 3) c3++;
        else if (d == 6) c6++;
        t = t / 11;
    }
    return c3 == c6;
}

static seq_t *cache = NULL;      /* stored valid terms */
static size_t cache_cap = 0;     /* current capacity of cache */
static size_t cache_len = 0;     /* number of terms cached */
static seq_t next_val = 0;       /* next integer to test */

/* grow the cache until it holds at least n terms */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        if (has_equal_36(next_val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(1);          /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_val;
        }
        next_val = next_val + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
