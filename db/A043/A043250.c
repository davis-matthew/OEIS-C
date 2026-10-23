/*
 * A043250  Numbers k such that 4 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Algorithm: brute–force scan to produce
    numbers k that contain the adjacent digit pair
    49 or 94, but not k-1.
    Upper bound on n (≈10⁴) is easily handled
    by iterating over integers in order.
    The function memoises all terms it has found
    so that subsequent calls are fast.  */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper: does x contain the pair 49 or 94? */
static bool has_pair(seq_t x)
{
    if (x < 10) return false;          /* single digit cannot contain a pair */
    int prev = (int)(x % 10);          /* least significant digit */
    x /= 10;
    while (x > 0) {
        int cur = (int)(x % 10);
        if ((prev == 9 && cur == 4) || (prev == 4 && cur == 9))
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

/* Main sequence generation function */
seq_t a(idx_t n)
{
    /* static state: cache of found terms and next integer to test */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;    /* allocated capacity of cache  */
    static size_t  len    = 0;    /* number of valid entries in cache */
    static seq_t  next    = 1;    /* next integer to test */

    /* if we already have enough terms, just return */
    if (len < (size_t)n) {
        /* ensure cache has at least n entries allocated */
        if (cap < (size_t)n) {
            size_t new_cap = cap ? cap << 1 : 16;
            if (new_cap < (size_t)n) new_cap = n;
            seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (tmp == NULL) { /* allocation failure; safe fallback */
                return 0;
            }
            if (cache) {
                for (size_t i = 0; i < len; ++i) tmp[i] = cache[i];
                free(cache);
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* continue scanning integers until we have n terms */
        while (len < (size_t)n) {
            seq_t val = next;
            if (has_pair(val) && !has_pair(val - 1)) {
                cache[len++] = val;
            }
            ++next;
        }
    }
    return cache[n - 1];   /* a indexing starts at 1 */
}
