/*
 * A044585  Numbers n such that string 2,1 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑8 representation
 *            contains the substring "21" but the base‑8 representation of n+1
 *            does not
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* has21 returns true iff the base‑8 representation of x contains
   consecutive digits 2 followed by 1. */
static bool has21(seq_t x)
{
    if (x == 0)
        return false;
    int last = -1;
    while (x > 0) {
        int d = (int)(x % 8);
        if (d == 2 && last == 1)
            return true;
        last = d;
        x /= 8;
    }
    return false;
}

/* a returns the n‑th term of A044585.  The sequence is generated
   lazily and cached in a static array so that successive calls are
   efficient. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* value for n<1 is undefined; return 0 */

    /* static cache of already generated terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_cnt = 0;  /* number of stored terms   */
    static seq_t last_checked = 0;/* last integer examined    */

    /* enlarge the cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* unrecoverable – terminate the program */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate missing terms */
    while (cached_cnt < n) {
        last_checked++;
        if (has21(last_checked) && !has21(last_checked + 1)) {
            cache[cached_cnt] = last_checked;
            cached_cnt++;
        }
    }

    return cache[n - 1];
}
