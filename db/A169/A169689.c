/*
 * A169689  (A169648(4n+4) - A147582(4n+5))/4.
 * https://oeis.org/A169689
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * a(-1) = 0, a(0) = 1, a(1) = 6.
 * For n >= 2, write n = 2^k + j with 0 <= j < 2^k.
 * Let jp1 = j + 1 and let r = floor_log2(jp1).
 * If jp1 is a power of two:
 *   if r < k   -> f = jp1
 *   if r == k  -> f = 3 * jp1
 *   if r > k   -> f = 0
 * otherwise f = 0.
 * Then a(n) = 3 * a(j) + f.
 */

#include <stdint.h>
#include <stdlib.h>

/* helper: floor(log2(x)) for x >= 1 */
static inline idx_t log2_floor(idx_t x)
{
    idx_t k = 0;
    while (x > 1) { x >>= 1; ++k; }
    return k;
}

static seq_t *cache = NULL;     /* stores a(-1), a(0), a(1), ... with offset +1 */
static idx_t cached_limit = 1;  /* maximum n for which a(n) is computed */

extern seq_t a(idx_t n)
{
    if (n < -1) return (seq_t)0;

    /* Initialize cache with base cases if needed */
    if (cache == NULL) {
        size_t init_sz = 3; /* indices -1, 0, 1 */
        cache = (seq_t*)calloc(init_sz, sizeof(seq_t));
        cache[0] = (seq_t)0;            /* a(-1) */
        cache[1] = (seq_t)1;            /* a(0)  */
        cache[2] = (seq_t)6;            /* a(1)  */
        cached_limit = 1;
    }

    /* Extend cache to cover requested n */
    if (n > cached_limit) {
        size_t new_size = (size_t)(n + 2); /* indices 0..n+1 */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failure */
        cache = tmp;

        for (idx_t cand = cached_limit + 1; cand <= n; ++cand) {
            idx_t k = log2_floor(cand);
            idx_t pow2 = (idx_t)1 << k;
            idx_t j   = cand - pow2;
            idx_t jp1 = j + 1;

            seq_t f;
            if ((jp1 & (jp1 - 1)) == 0) {           /* jp1 is a power of two */
                idx_t r = log2_floor(jp1);
                if (r == k)           f = (seq_t)3 * (seq_t)jp1;
                else if (r < k)       f = (seq_t)jp1;
                else                  f = 0;
            } else {
                f = 0;
            }

            seq_t aj = cache[j + 1];
            seq_t an = (seq_t)3 * aj + f;
            cache[cand + 1] = an;
        }
        cached_limit = n;
    }

    return cache[n + 1];
}
