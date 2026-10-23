/*
 * A034308  Decimal part of a(n)^(1/2) starts with reversal of its integer
 *          part: first term of runs.
 * https://oeis.org/A034308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the A034308 sequence: the first term of each run of integers
 * whose square‑root has a decimal part starting with the reversal of its
 * integer part.
 * For each integer part k (k ≥ 1) we find the interval
 *
 *   k^2 ≤ n < (k+1)^2
 *   and   floor(10^d · (sqrt(n) - k)) = rev(k)      (d = digits(k))
 *
 * The first integer of the interval is stored as the next sequence term.
 * The sequence is generated lazily and cached for random access.
 */
#include <stdlib.h>

static inline idx_t rev_idx(idx_t x) {
    idx_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x /= 10;
    }
    return rev;
}

static inline idx_t digits_idx(idx_t x) {
    idx_t d = 0;
    do {
        ++d;
        x /= 10;
    } while (x);
    return d;
}

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;       /* next integer part to test */

    /* return cached value if already computed */
    if (n < cache_size) return cache[n];

    /* generate terms until we have at least n+1 values */
    while (cache_size <= n) {
        idx_t k = next_k++;                     /* integer part of sqrt */
        idx_t rev = rev_idx(k);
        idx_t d = digits_idx(k);

        /* pow10 = 10^d */
        seq_t pow10 = 1;
        for (idx_t i = 0; i < d; ++i)
            pow10 *= (seq_t)10;

        seq_t pow10Sq = pow10 * pow10;

        seq_t kSeq = k;
        seq_t kSq = kSeq * kSeq;
        seq_t k1Sq = (kSeq + (seq_t)1) * (kSeq + (seq_t)1);
        seq_t upperInt = k1Sq - (seq_t)1;       /* (k+1)^2 - 1 */

        /* a = k·pow10 + rev  (numerator of k + rev/pow10) */
        seq_t a_val = kSeq * pow10 + (seq_t)rev;

        /* lower bound: ceil((a_val / pow10)^2) = ceil((a_val^2)/pow10Sq) */
        seq_t lowerRaw = (a_val * a_val + pow10Sq - (seq_t)1) / pow10Sq;

        /* upper bound: floor(((a_val + 1)/pow10)^2 - ε) */
        seq_t a_plus = a_val + (seq_t)1;        /* corresponds to rev+1 */
        seq_t upperRaw = ((a_plus * a_plus) - (seq_t)1) / pow10Sq;

        /* intersect with [k^2, (k+1)^2-1] */
        seq_t L = (lowerRaw > kSq) ? lowerRaw : kSq;
        seq_t R = (upperRaw < upperInt) ? upperRaw : upperInt;

        if (L <= R) {                          /* run exists */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                if (new_cap < cache_size + 1) new_cap = cache_size + 1;
                seq_t *tmp = (seq_t*)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = L;
        }
    }

    return cache[n];
}
