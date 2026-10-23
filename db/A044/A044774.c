/*
 * A044774  Numbers n such that string 6,1 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that string 61 occurs in base 10 representation
 *            of n but not of n+1
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* array of found terms */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static size_t cache_len = 0;         /* number of terms stored */
static idx_t pos = 1;                /* next integer to test */

/* ensure cache can hold at least `need` elements */
static void ensure_capacity(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* true if decimal representation of x contains the substring "61" */
static bool has61(seq_t x) {
    seq_t prev = 0;                    /* lower‐order digit seen so far */
    while (x > 0) {
        seq_t d = x % 10;
        if (d == 6 && prev == 1) return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset starts at 1 */
    while (cache_len < (size_t)n) {
        seq_t cur = (seq_t)pos;
        if (has61(cur) && !has61(cur + 1)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = cur;
        }
        pos++;
    }
    return cache[n - 1];
}
