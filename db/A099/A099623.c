/*
 * A099623  a(n) = Sum_{k=0..floor(n/2)} C(n-k,k+2)*2^(n-k-2)*(3/2)^k.
 * https://oeis.org/A099623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) - 4*a(n-3) + 12*a(n-4) with initial
 *            values a(0)=0, a(1)=0, a(2)=1, a(3)=6
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_CACHE 65536   /* large enough for all n up to ~10 000 */

static seq_t cache[MAX_CACHE];
static idx_t cache_size = 0;

/* Compute terms up to index n, using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;                     /* already computed */

    /* Initialise missing initial terms if necessary. */
    if (cache_size == 0) {
        cache[0] = 0;
        ++cache_size;
    }
    if (cache_size == 1) {
        cache[1] = 0;
        ++cache_size;
    }
    if (cache_size == 2) {
        cache[2] = 1;
        ++cache_size;
    }
    if (cache_size == 3) {
        cache[3] = 6;
        ++cache_size;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = 6 * cache[i-1];
        seq_t t2 = 9 * cache[i-2];
        seq_t t3 = 4 * cache[i-3];
        seq_t t4 = 12 * cache[i-4];
        cache[i] = t1 - t2 - t3 + t4;
        ++cache_size;
    }
}

seq_t a(idx_t n)
{
    if (n >= MAX_CACHE) {
        /* n is out of the pre‑allocated range; fall back to an
           on‑demand computation without caching. */
        seq_t prev4 = 0, prev3 = 0, prev2 = 1, prev1 = 6;
        if (n == 0) return prev4;
        if (n == 1) return prev3;
        if (n == 2) return prev2;
        if (n == 3) return prev1;
        for (idx_t i = 4; i <= n; ++i) {
            seq_t cur = 6 * prev1 - 9 * prev2 - 4 * prev3 + 12 * prev4;
            prev4 = prev3; prev3 = prev2; prev2 = prev1; prev1 = cur;
        }
        return prev1;
    }
    ensure_cache(n);
    return cache[n];
}
