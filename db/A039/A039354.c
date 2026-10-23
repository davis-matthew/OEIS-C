/*
 * A039354  Numbers whose base-8 representation has the same nonzero number of
 *          5's and 6's.
 * https://oeis.org/A039354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (search): numbers whose base‑8 representation has the same
   non‑zero number of 5's and 6's.  The function iteratively checks
   integers and caches results to allow arbitrary order of calls. */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* static state for memoisation */
static seq_t *cache       = NULL;     /* array of computed terms   */
static size_t cacheSize   = 0;        /* number of terms stored    */
static size_t cacheCap    = 0;        /* current array capacity    */
static seq_t nextCheck    = 1;        /* next integer to test      */

/* test whether x has equal non‑zero numbers of digits 5 and 6 in base 8 */
static inline bool valid(seq_t x)
{
    seq_t cnt5 = 0, cnt6 = 0;
    seq_t t = x;
    if (t == 0) return false;               /* 0 has no 5 or 6 */
    while (t > 0) {
        seq_t d = t % 8;
        if (d == 5) cnt5++;
        else if (d == 6) cnt6++;
        t = t / 8;
    }
    return (cnt5 == cnt6) && (cnt5 > 0);
}

/* enlarge the cache array so that it can hold at least `need` elements */
static void ensureCapacity(size_t need)
{
    if (need <= cacheCap) return;
    size_t newCap = cacheCap ? cacheCap : 16;
    while (newCap < need) newCap <<= 1;
    /* portable cast; works in both C and C++ for types that can be
       trivially copied.  For non‑trivial class types the build must
       provide a trivial constructor (as expected for the big integer
       class used in the project). */
    seq_t *p = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
    if (p == NULL && newCap > 0) abort();   /* allocation failure */
    cache = p;
    cacheCap = newCap;
}

/* required function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; undefined for n<1 */

    size_t need = (size_t)n;                 /* number of terms to compute */

    if (cache == NULL) {
        cacheCap = 16;
        cache = (seq_t *)malloc(cacheCap * sizeof(seq_t));
        if (cache == NULL && cacheCap > 0) abort();
        cacheSize = 0;
    }

    ensureCapacity(need);

    while (cacheSize < need) {
        if (valid(nextCheck)) {
            cache[cacheSize++] = nextCheck;
        }
        nextCheck = nextCheck + 1;    /* move to next integer */
    }
    return cache[need - 1];
}
