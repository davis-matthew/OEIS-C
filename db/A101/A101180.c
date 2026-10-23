/*
 * A101180  Numbers n such that 19*n^2 + 19*n + 1 is a square.
 * https://oeis.org/A101180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101180: Numbers n such that 19*n^2 + 19*n + 1 is a square.
   Recurrence for n > 8:
     a(n) = 57799 * (2 * a(n-4) + 1) - a(n-8) - 1
   Initial values:
     a(1)=0, a(2)=8, a(3)=671, a(4)=15639,
     a(5)=42159, a(6)=981911, a(7)=77624048,
     a(8)=1807894920
*/
#include <stdlib.h>

static const seq_t init[8] = {
    0,                     // a(1)
    8,                     // a(2)
    671,                   // a(3)
    15639,                 // a(4)
    42159,                 // a(5)
    981911,                // a(6)
    77624048,              // a(7)
    1807894920             // a(8)
};

static seq_t *cache      = NULL;
static idx_t  cache_len  = 0;
static const seq_t K = 57799;

/* Ensure that cache contains values up to a(n) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    if (cache == NULL) {
        /* start with the first 8 terms */
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];
    }

    idx_t old_len = cache_len;          /* length before growth */
    cache_len = n;                      /* new wanted length */
    cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));

    for (idx_t i = old_len + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i - 1] = init[i - 1];
        } else {
            seq_t t = ((seq_t)2 * cache[i - 5]) + 1;
            cache[i - 1] = K * t - cache[i - 9] - 1;
        }
    }
}

seq_t a(idx_t n)
{
    /* sequence indexed starting at 1 */
    ensure_cache(n);
    return cache[n - 1];
}
