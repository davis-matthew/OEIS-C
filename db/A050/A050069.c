/*
 * A050069  a(n) = a(n-1) + a(m) for n >= 3, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1 and a(2) = 3.
 * https://oeis.org/A050069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 70, u32 n <= 514, u64 n <= 4271,
 *            u128 n <= 8193, big any n
 * Helpers:   p_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=3;
   for n≥3, a(n)=a(n-1)+a(m) where
   m = 2^(p+1) + 2 – n
   and p is the unique integer with 2^p < n‑1 ≤ 2^(p+1). */

#include <stdint.h>
#include "../utils/p_floor.h"

#define MAX_CACHE 20001   /* enough for all tests up to ~10000 */

/* static storage of already‑computed terms (0‑based index) */
static seq_t cache[MAX_CACHE];
static unsigned int cache_filled = 0;

/* initialise constants; called lazily on the first request */
static void init_cache(void) {
    if (cache_filled == 0) {
        cache[0] = (seq_t)1;  /* a(1) */
        cache[1] = (seq_t)3;  /* a(2) */
        cache_filled = 2;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* not part of the sequence */
    init_cache();

    /* extend the cache up to the requested n */
    while (cache_filled < n) {
        idx_t i = cache_filled + 1;          /* current n (1‑based) */
        unsigned p = p_floor(i - 2);          /* p for this n */

        seq_t pow2 = (seq_t)1;
        pow2 <<= (p + 1);                     /* 2^(p+1) */
        idx_t m = (idx_t)pow2 + 2 - i;        /* m = 2^(p+1) + 2 – n */

        /* store a(i) = a(i-1) + a(m) */
        cache[cache_filled] = cache[cache_filled - 1] + cache[m - 1];
        ++cache_filled;
    }

    return cache[n - 1];
}
