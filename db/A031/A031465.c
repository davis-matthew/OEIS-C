/*
 * A031465  Numbers whose base-4 representation has 4 more 0's than 3's.
 * https://oeis.org/A031465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 986, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031465: numbers whose base‑4 representation has 4 more 0's than 3's.
   The algorithm enumerates integers, counts 0 and 3 digits in base‑4 and
   retains those with a difference of 4.  Results are memoised in a
   dynamically‑resized array of idx_t values and returned as seq_t. */

#include <stdlib.h>

/* Static cache holding the accepted numbers as idx_t values.  We store the
   values in idx_t to avoid constructing `seq_t` objects via malloc/realloc
   (these may be arbitrary‑precision classes).  Casting to seq_t is done only
   on return. */
static idx_t *cache = NULL;
static idx_t cache_size   = 0;   /* number of entries currently cached   */
static idx_t cache_cap    = 0;   /* allocated capacity                  */
static idx_t last_checked = 0;   /* highest integer examined already     */

/* Ensure the cache has room for at least 'need' entries. */
static void ensure_capacity(idx_t need)
{
    if (cache_cap < need) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1024;
        if (new_cap < need) new_cap = need;
        /* Cast for C++ safety; malloc/realloc are allowed in both C and C++. */
        idx_t *tmp = (idx_t *)realloc(cache, sizeof(idx_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of A031465 (n >= 1). */
seq_t a(idx_t n)
{
    /* If already cached, return immediately. */
    if (n <= cache_size) {
        return (seq_t)cache[n - 1];
    }

    /* Extend the cache until we have at least n terms. */
    while (cache_size < n) {
        ++last_checked;                     /* next candidate integer   */
        idx_t x = last_checked;
        int zeros = 0;
        int threes = 0;

        /* Count 0's and 3's in the base‑4 representation of x. */
        do {
            int digit = x & 3;              /* x % 4 */
            if (digit == 0) {
                ++zeros;
            } else if (digit == 3) {
                ++threes;
            }
            x >>= 2;                         /* x /= 4 */
        } while (x > 0);

        if (zeros - threes == 4) {
            ensure_capacity(cache_size + 1);
            cache[cache_size] = last_checked;
            ++cache_size;
        }
    }

    return (seq_t)cache[n - 1];
}
