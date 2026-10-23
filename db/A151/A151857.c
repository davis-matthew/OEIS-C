/*
 * A151857  a(0)=1; a(1)=1; a(2)=3; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=a(j) + 2*a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A151857
   a(0)=1, a(1)=1, a(2)=3;
   for n>=3 if n = 2^i + j with 0<=j<2^i
   then a(n) = a(j) + 2*a(j+1); if j = 2^i-1 add 1 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* cache for memoisation */
#ifdef __cplusplus
static std::vector<seq_t> CACHE;
static std::vector<bool>  COMPUTED;
#else
static seq_t *CACHE = NULL;
static char *COMPUTED = NULL;
static size_t CACHE_CAP = 0;
#endif

/* resize cache so that indices up to n are valid */
static void ensure_cache(idx_t n)
{
#ifdef __cplusplus
    if (CACHE.size() <= n) {
        CACHE.resize(n + 1);
        COMPUTED.resize(n + 1);    /* new entries are false */
    }
#else
    size_t needed = (size_t)n + 1;
    if (CACHE_CAP < needed) {
        size_t new_cap = CACHE_CAP ? CACHE_CAP * 2 : 8;
        if (new_cap < needed) new_cap = needed;

        seq_t *tmp  = (seq_t *)realloc(CACHE, new_cap * sizeof(seq_t));
        char *tmpc  = (char *)realloc(COMPUTED, new_cap * sizeof(char));
        if (!tmp || !tmpc)
            return; /* allocation failed – leave cache unchanged */

        if (CACHE) {
            /* zero new slots to indicate “not computed” */
            memset(tmp  + CACHE_CAP, 0, (new_cap - CACHE_CAP) * sizeof(seq_t));
            memset(tmpc + CACHE_CAP, 0, (new_cap - CACHE_CAP) * sizeof(char));
        } else {
            memset(tmp, 0, new_cap * sizeof(seq_t));
            memset(tmpc, 0, new_cap * sizeof(char));
        }
        CACHE     = tmp;
        COMPUTED = tmpc;
        CACHE_CAP = new_cap;
    }
#endif
}

/* recursive definition with memoisation */
seq_t a(idx_t n)
{
    /* base values */
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)3;

    /* ensure cache slot exists */
    ensure_cache(n);

    /* return cached value if already computed */
#ifdef __cplusplus
    if (COMPUTED[n])
        return CACHE[n];
#else
    if (COMPUTED[n])
        return CACHE[n];
#endif

    /* find largest power of two ≤ n */
    idx_t pow = 1;
    while ((pow << 1) <= n)
        pow <<= 1;

    idx_t j = n - pow;   /* 0 ≤ j < pow */

    seq_t aj  = a(j);
    seq_t aj1 = a(j + 1);

    seq_t res = aj + (seq_t)2 * aj1;
    if (j == pow - 1)
        res += (seq_t)1;

#ifdef __cplusplus
    CACHE[n] = res;
    COMPUTED[n] = true;
#else
    CACHE[n] = res;
    COMPUTED[n] = 1;
#endif
    return res;
}
