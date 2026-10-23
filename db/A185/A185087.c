/*
 * A185087  a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k)*A000108(k+1).
 * https://oeis.org/A185087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 51, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A185087
   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n-k,k) * Catalan(k+1)
   Recurrence: (n+4)*a(n) = (2n+5)*a(n-1) + 3(n+1)*a(n-2)
                        – 2(2n-1)*a(n-3)
   Initial values: a(0)=1, a(1)=1, a(2)=3
*/
#include <stdlib.h>

/* Cache for memoised values  */
static seq_t *cache   = NULL;   /* array of computed a(i) */
static idx_t  cached_len = 0;   /* number of filled entries     */
static idx_t  cached_cap = 0;   /* capacity of allocated array   */

/* enlarge capacity to hold at least (n+1) entries */
static void ensure_capacity(idx_t n)
{
    if (cached_cap <= n) {
        idx_t newcap = cached_cap ? cached_cap : 4;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);          /* out of memory */
        cache = tmp;
        cached_cap = newcap;
    }
}

/* Function computing a(n) with memoisation and the above recurrence */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* compute missing terms up to n */
    while (cached_len <= n) {
        idx_t i = cached_len;     /* we are computing a(i) */

        if (i == 0) {             /* base case a(0) */
            cache[0] = (seq_t)1;
        }
        else if (i == 1) {         /* base case a(1) */
            cache[1] = (seq_t)1;
        }
        else if (i == 2) {         /* base case a(2) */
            cache[2] = (seq_t)3;
        }
        else {                     /* recurrence for i >= 3 */
#ifdef __SIZEOF_INT128__
            /* for builtin integer types (≤64 bit) use 128‑bit intermediates to avoid overflow */
            if (sizeof(seq_t) <= 8) {
                __int128 term1 = (__int128)cache[i-1] * (2*i + 5);
                __int128 term2 = (__int128)cache[i-2] * (3*(i + 1));
                __int128 term3 = (__int128)cache[i-3] * (2*(2*i - 1));
                __int128 numer = term1 + term2 - term3;
                cache[i] = (seq_t)(numer / (i + 4));
            } else
#endif
            {
                /* generic (class) big‑int or 128‑bit case: normal arithmetic operators */
                seq_t term1 = cache[i-1] * (2*i + 5);
                seq_t term2 = cache[i-2] * (3*(i + 1));
                seq_t term3 = cache[i-3] * (2*(2*i - 1));
                seq_t numer = term1 + term2 - term3;
                cache[i] = numer / (i + 4);
            }
        }
        cached_len = i + 1;        /* next index to compute */
    }

    return cache[n];
}
