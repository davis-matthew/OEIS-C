/*
 * A080595  Consider the standard game of Nim with 3 heaps and make a list of
 *          the losing positions (x,y,z) with x <= y <= z in reverse
 *          lexicographic order; sequence gives z values.
 * https://oeis.org/A080595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the z-coordinate of the n-th Nim losing position (x,y,z)
 *            with x ≤ y ≤ z, enumerated by increasing z and for each fixed z
 *            by increasing y. This function generates the positions directly
 *            without an explicit formula
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_z = 0;

/* Ensure capacity for at least 'need' elements. */
static void ensure_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Append a z value to the cache. */
static void push_z(idx_t z)
{
    ensure_cap(cache_len + 1);
    cache[cache_len++] = (seq_t)z;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n+1 entries. */
    while (cache_len <= n) {
        /* For the current z, enumerate y from 0..z; the associated
           x = y ^ z satisfies x <= y iff (x,y,z) is a losing position. */
        for (idx_t y = 0; y <= next_z; ++y) {
            idx_t x = y ^ next_z;
            if (x <= y) {
                push_z(next_z);
            }
        }
        ++next_z;
    }
    return cache[n];
}
