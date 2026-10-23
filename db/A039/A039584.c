/*
 * A039584  Numbers whose base-6 representation has the same number of 1's,
 *          2's and 4's.
 * https://oeis.org/A039584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑6 representation has the same number of
   1's, 2's, and 4's.  Computed by brute‑force search. */

#include <stdbool.h>
#include <stdlib.h>

/* Test whether a number has equal counts of 1, 2 and 4 in base 6. */
static bool is_good(seq_t v)
{
    if (v == 0)
        return true;           /* 0 has 0 of each digit */

    int c1 = 0, c2 = 0, c4 = 0;
    for (seq_t t = v; t > 0; t /= 6) {
        int d = (int)(t % 6);
        if (d == 1)
            ++c1;
        else if (d == 2)
            ++c2;
        else if (d == 4)
            ++c4;
    }
    return (c1 == c2) && (c2 == c4);
}

/* Cache state: static variables that must be visible to all helper
   functions. */
static seq_t *cache = NULL;     /* array of stored terms, 1‑based */
static size_t cache_len = 0;    /* number of stored terms */
static size_t cache_cap = 0;    /* allocated capacity */
static seq_t next_start = 0;    /* next integer to test */

/* Ensure the cache array has room for at least one more element. */
static void grow_cache(void)
{
    if (cache_len < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* In a real implementation we would check new_arr != NULL,
       but for the exercise we assume success. */
    cache = new_arr;
    cache_cap = new_cap;
}

/* Return the n‑th term (n ≥ 1).  The sequence is generated on demand and
   the results are memoised, so subsequent calls are fast. */
seq_t a(idx_t n)
{
    if (n == 0)              /* undefined by OEIS offset, but safe */
        return 0;

    /* Produce terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (is_good(next_start)) {
            grow_cache();
            cache[cache_len++] = next_start;
        }
        ++next_start;
    }
    return cache[n - 1];     /* a(1) is stored at index 0 */
}
