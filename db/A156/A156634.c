/*
 * A156634  Denominator of Euler(n, 1/18).
 * https://oeis.org/A156634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
   Denominator of Euler(n, 1/18)  
   The denominator equals 18^n for even n, and 18^n/8 for odd n ≥ 3.  
   Special values: a(0)=1, a(1)=9.  
   The implementation pre‑computes powers of 18 on demand and reuses
   them for all calls.
*/
#include <stdlib.h>

static seq_t *pow18_cache = NULL;   /* array of 18^i values */
static size_t cache_cap   = 0;      /* allocated size of the array   */
static idx_t  cache_max   = 0;      /* highest index that has been computed */

static void ensure_pow18(idx_t n)
{
    /* initialize cache if not done yet */
    if (!pow18_cache) {
        cache_cap = 1;
        pow18_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!pow18_cache) abort();
        pow18_cache[0] = 1;          /* 18^0 */
        cache_max = 0;
    }

    /* grow the cache to reach index n */
    while (cache_max < n) {
        /* grow the allocated array if needed */
        if (cache_max + 1 >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 1;
            seq_t *tmp = (seq_t *)realloc(pow18_cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            pow18_cache = tmp;
            cache_cap = new_cap;
        }

        /* compute next power of 18 */
        seq_t next = pow18_cache[cache_max] * (seq_t)18;
        cache_max += 1;
        pow18_cache[cache_max] = next;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* compute powers up to n if necessary */
    ensure_pow18(n);

    seq_t val = pow18_cache[n];
    if ((n & 1) == 0) /* even n */
        return val;
    else              /* odd n ≥ 3 */
        return val / (seq_t)8;
}
