/*
 * A186567  Number of (n+2)X10 0..2 arrays with every 3X3 subblock commuting
 *          with each horizontal and vertical neighbor 3X3 subblock.
 * https://oeis.org/A186567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 33, u64 n <= 85, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186567: Number of (n+2)×10 0..2 arrays with every 3×3 subblock
   commuting with each horizontal and vertical neighbor 3×3 subblock.
   Recurrence for n>16:
       a(n)=2*a(n-1)-a(n-2)+2*a(n-3)-4*a(n-4)+2*a(n-5)+2*a(n-6)-2*a(n-7)
   Initial values (a(1)..a(16)):
       35198, 4795, 8029, 18415, 33445, 44887, 60973, 82783,
       112357, 168727, 255085, 381007, 587317, 891271, 1333501, 2082015
   The function memoises terms so that repeated calls are O(1).
*/

#include <stdlib.h>
#include <string.h>

static const seq_t init_vals[17] = {
    0,          /* unused 0 index */
    35198,      /* a(1) */
    4795,       /* a(2) */
    8029,       /* a(3) */
    18415,      /* a(4) */
    33445,      /* a(5) */
    44887,      /* a(6) */
    60973,      /* a(7) */
    82783,      /* a(8) */
    112357,     /* a(9) */
    168727,     /* a(10) */
    255085,     /* a(11) */
    381007,     /* a(12) */
    587317,     /* a(13) */
    891271,     /* a(14) */
    1333501,    /* a(15) */
    2082015     /* a(16) */
};

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> cache;          /* memoisation storage (index 0 unused) */
#else
static seq_t *cache = NULL;              /* memoisation storage (index 0 unused) */
static idx_t cache_len = 0;              /* allocated entries count (>=0) */
#endif

/* Ensure that cache can hold indices 0..n (size n+1).  Newly added entries are zero‑initialised. */
static void ensure_cache_size(idx_t n)
{
#ifdef __cplusplus
    if (cache.size() > n) return;
    cache.resize(n + 1);
#else
    if (cache_len > n) return;
    idx_t new_size = n + 1;                      /* at least n+1 entries */
    seq_t *new_ptr = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (new_ptr == NULL) exit(1);                /* allocation failed */
    if (cache == NULL) {                         /* first allocation: zero‑init all */
        memset(new_ptr, 0, new_size * sizeof(seq_t));
    } else {                                     /* zero the newly added part only */
        size_t added = new_size - cache_len;
        memset(new_ptr + cache_len, 0, added * sizeof(seq_t));
    }
    cache     = new_ptr;
    cache_len = new_size;
#endif
}

/* Compute the n‑th term (n≥1).  The result is returned as seq_t. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined for n≤0, return 0 */

    if (n <= 16) return init_vals[n];          /* initial segment known */

    ensure_cache_size(n);

    /* Fill cache up to n if not already present. */
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 16) {                        /* initialise first 16 entries */
#ifdef __cplusplus
            if (cache[i] == 0) cache[i] = init_vals[i];
#else
            if (cache[i] == 0) cache[i] = init_vals[i];
#endif
        } else {                              /* recurrence for i>16 */
#ifdef __cplusplus
            if (cache[i] == 0) {
                seq_t t = 2 * cache[i-1];
                t -= cache[i-2];
                t += 2 * cache[i-3];
                t -= 4 * cache[i-4];
                t += 2 * cache[i-5];
                t += 2 * cache[i-6];
                t -= 2 * cache[i-7];
                cache[i] = t;
            }
#else
            if (cache[i] == 0) {
                seq_t t = 2 * cache[i-1];
                t -= cache[i-2];
                t += 2 * cache[i-3];
                t -= 4 * cache[i-4];
                t += 2 * cache[i-5];
                t += 2 * cache[i-6];
                t -= 2 * cache[i-7];
                cache[i] = t;
            }
#endif
        }
    }

    return cache[n];
}
