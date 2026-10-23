/*
 * A194394  Numbers m such that Sum_{k=1..m} (<1/2 + k*r> - <k*r>) > 0, where
 *          r=sqrt(13) and < > denotes fractional part.
 * https://oeis.org/A194394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers m such that the cumulative sum of
 *  +1 when {k·sqrt(13)} < 1/2 and -1 otherwise
 *  is positive.  Equivalently, m is the index
 *  at which the running sum of the sequence
 *  (+1 or -1) first becomes > 0.
 *
 *  The implementation iterates k = 1,2,3,…,
 *  keeps the running sum, and stores each k
 *  that yields a positive sum.  The results
 *  are cached so that calls to a(n) need not
 *  recompute earlier terms.
 */

#include <stdint.h>
#include <stdlib.h>

/* integer square root (floor) for 64‑bit values */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = 0xFFFFFFFFULL;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t sq  = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* static cache for already computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* state for the running process */
    static int64_t sum   = 0;      /* cumulative (+1/-1) sum   */
    static uint64_t k_next = 1;    /* next k to be processed   */

    while (cache_len < n) {
        uint64_t k     = k_next++;
        uint64_t rhs   = k * k * 13ULL;                  /* k²·13                     */
        uint64_t F     = isqrt_u64(rhs);               /* floor(k*sqrt(13))         */
        uint64_t lhs   = 4ULL * k * k * 13ULL;          /* 4·k²·13                   */
        uint64_t t     = 2ULL * F + 1ULL;               /* 2·F+1                     */
        uint64_t comp  = t * t;                         /* (2·F+1)²                   */

        /* +1 if fractional part < 1/2, else -1 */
        if (lhs < comp)
            ++sum;
        else
            --sum;

        if (sum > 0) {
            /* enlarge cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_arr) abort();
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
