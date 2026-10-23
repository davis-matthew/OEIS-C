/*
 * A010917  Pisot sequence E(5,21), a(n) = floor( a(n-1)^2/a(n-2) + 1/2 ).
 * https://oeis.org/A010917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A010917: Pisot sequence E(5,21)
   a(0) = 5, a(1) = 21
   a(n) = floor( a(n-1)^2 / a(n-2) + 1/2 )
   for n ≥ 2
*/

#include <stdlib.h>   /* calloc, realloc, exit */

#ifndef __cplusplus
/* C implementation – use unsigned __int128 for safe intermediate arithmetic */
static seq_t round_div_sq(seq_t a, seq_t b)
{
    unsigned __int128 aa   = (unsigned __int128)a * a;
    unsigned __int128 num  = aa * 2 + (unsigned __int128)b;
    unsigned __int128 den  = (unsigned __int128)b * 2;
    return (seq_t)(num / den);
}
#else
/* C++ implementation – generic integer arithmetic works for native
   integers and arbitrary‑precision types */
static seq_t round_div_sq(seq_t a, seq_t b)
{
    seq_t aa   = a * a;
    seq_t q    = aa / b;
    seq_t r    = aa % b;
    seq_t half = b / 2;
    /* round up if remainder > half or exact half in even case */
    if ( (r > half) || (r == half && (b % 2 == 0)) )
        ++q;
    return q;
}
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static size_t  last  = 1;   /* last computed index (initially 1) */

    if (cache == NULL) {
        cap   = 2;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        cache[0] = 5;
        cache[1] = 21;
    }

    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    if ((size_t)n >= cap) {
        size_t newcap = cap * 2;
        while (newcap <= (size_t)n) newcap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);   /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    for (idx_t i = (idx_t)last + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t prevprev = cache[i - 2];
        cache[i] = round_div_sq(prev, prevprev);
    }
    last = (size_t)n;
    return cache[n];
}
