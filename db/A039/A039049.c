/*
 * A039049  Numbers whose base-7 representation has the same number of 5's and
 *          6's.
 * https://oeis.org/A039049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-7 representation has the same number of 5's and 6's.
   The function scans decimal integers, keeping only those whose base‑7 digits contain
   an equal count of 5 and 6.  The terms are cached so that any call to a(n)
   is answered in O(n) time overall for n up to about 10000. */

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *cache      = NULL; /* memoised terms */
static size_t cache_cap  = 0;    /* capacity of `cache` */
static size_t cache_sz   = 0;    /* number of stored terms */
static idx_t  next_num   = 0;    /* next decimal candidate to test */

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – terminate, as the sequence cannot be computed. */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate terms until at least `n` terms are stored. */
static void generate_upto(idx_t n)
{
    if (cache_sz >= n)
        return;

    ensure_cache_capacity(n + 16);  /* spare capacity for future growth */

    while (cache_sz < n) {
        idx_t x = next_num;
        int c5 = 0, c6 = 0;

        if (x != 0) {
            idx_t y = x;
            while (y) {
                int digit = (int)(y % 7);
                if (digit == 5)
                    ++c5;
                else if (digit == 6)
                    ++c6;
                y /= 7;
            }
        }
        /* 0 has zero 5's and zero 6's, so it satisfies the condition. */

        if (c5 == c6) {
            cache[cache_sz++] = (seq_t)next_num;
        }
        ++next_num;
    }
}

/* Public entry point required by the exerciser. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 0; return 0 for safety */
        return 0;
    generate_upto(n);
    return cache[n - 1];
}
