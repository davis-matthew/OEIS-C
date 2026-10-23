/*
 * A043887  Numbers k such that 0 and 2 occur juxtaposed in the base-5
 *          representation of k but not of k+1.
 * https://oeis.org/A043887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043887 – Numbers k such that 0 and 2 occur juxtaposed (in any order) in the base‑5 representation of k, but not in k+1.
   The sequence is computed by searching for such k. The search is memoized between calls. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;     /* array of found terms */
static idx_t  cap   = 0;        /* current capacity of cache */
static idx_t  cnt   = 0;        /* number of terms currently stored */
static idx_t  next  = 0;        /* next integer to test */

/* return true iff base‑5 representation of x contains adjacent digits 0 and 2 (in either order) */
static bool has_adj02(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % (seq_t)5);
        if (prev != -1 && ((prev == 0 && d == 2) || (prev == 2 && d == 0)))
            return true;
        prev = d;
        x = x / (seq_t)5;
    }
    return false;
}

/* grow cache if needed */
static void grow_cache(void)
{
    if (cnt >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* out of memory – terminate */
        cache = tmp;
        cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n=0, but never called */

    while (cnt < n) {
        seq_t k = (seq_t)next;
        if (has_adj02(k) && !has_adj02(k + (seq_t)1)) {
            grow_cache();
            cache[cnt++] = k;
        }
        next++;
    }
    return cache[n - 1];
}
