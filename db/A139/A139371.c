/*
 * A139371  Let the binary expansion of n be n = Sum_{k} 2^{r_k}, let e(n) be
 *          the number of r_k's that are even, o(n) the number that are odd;
 *          sequence lists n such that e(n) <= o(n).
 * https://oeis.org/A139371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   integers n such that e(n)<=o(n), listed in increasing order
 * Range:     u8 n <= 163, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cacheSize = 0;
static size_t cacheCap = 0;
static seq_t nextVal = 0;

static void ensure_cap(size_t needed) {
    if (needed <= cacheCap) return;
    size_t newCap = cacheCap ? cacheCap * 2 : 64;
    while (newCap < needed) newCap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
    cache = tmp;
    cacheCap = newCap;
}

static int good(seq_t v) {
    size_t even = 0, odd = 0;
    size_t pos = 0;
    while (v > 0) {
        if (v & 1) {
            if ((pos & 1) == 0) even++;
            else odd++;
        }
        v >>= 1;
        pos++;
    }
    /* For v == 0, even==odd==0 -> condition holds */
    return even <= odd;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* behaviour below offset not required */
    size_t idx = (size_t)n;       /* cast for array indexing */

    while (cacheSize < idx) {
        if (good(nextVal)) {
            ensure_cap(cacheSize + 1);
            cache[cacheSize++] = nextVal;
        }
        nextVal++;
    }
    return cache[idx - 1];
}
