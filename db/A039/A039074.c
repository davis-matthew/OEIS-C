/*
 * A039074  Numbers whose base-8 representation has the same number of 5's and
 *          6's.
 * https://oeis.org/A039074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th integer whose base‑8 representation has equal
 *            numbers of 5 and 6
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cache of found terms */
static seq_t *cache = NULL;          /* array of terms */
static size_t cache_cap = 0;         /* current capacity of cache */
static size_t cache_used = 0;        /* how many terms are stored */
static seq_t next_candidate = 0;     /* next integer to test */

/* True if x's base‑8 digits contain the same count of 5’s and 6’s */
static bool is_good(seq_t x)
{
    unsigned int cnt5 = 0, cnt6 = 0;
    while (x > 0)
    {
        unsigned int d = (unsigned int)(x & (seq_t)7);   /* take last octal digit */
        if (d == 5) ++cnt5;
        else if (d == 6) ++cnt6;
        x >>= 3;   /* divide by 8 */
    }
    return cnt5 == cnt6;
}

/* Make sure the cache contains at least `need` terms (1‑based index). */
static void ensure_cache(idx_t need)
{
    while (cache_used < (size_t)need)
    {
        if (is_good(next_candidate))
        {
            if (cache_used == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();              /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_used++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* Sequence function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* undefined for n==0, but safe guard */
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
