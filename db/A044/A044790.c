/*
 * A044790  Numbers n such that string 7,7 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number n such that the decimal representation of n contains the substring
   "77", but n+1 does not.  The sequence is computed lazily with state
   preserved between calls. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Test if x contains the consecutive digits 77 in decimal. */
static bool has77(seq_t x)
{
    if (x < 10)
        return false;
    while (x >= 10) {
        if (x % 100 == 77)
            return true;
        x /= 10;
    }
    return false;
}

/* Grow the cache array to hold at least 'need' elements. */
static void grow_cache(size_t need, seq_t **array, size_t *cap)
{
    if (*cap >= need)
        return;
    size_t newcap = *cap ? (*cap * 2) : 8;
    while (newcap < need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)malloc(newcap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    if (*array)
        memcpy(tmp, *array, (*cap) * sizeof(seq_t));
    free(*array);
    *array = tmp;
    *cap = newcap;
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of acquired terms */
    static size_t cache_len = 0;      /* number of terms stored     */
    static size_t cache_cap = 0;      /* capacity of the array      */
    static seq_t cur = 1;             /* smallest integer to test   */

    if (n <= cache_len)
        return cache[n - 1];

    for (;;) {
        if (has77(cur) && !has77(cur + 1)) {
            grow_cache(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = cur;
            if (n == cache_len) {
                seq_t result = cur;
                cur++;        /* advance cursor for next call */
                return result;
            }
        }
        cur++;
    }
}
