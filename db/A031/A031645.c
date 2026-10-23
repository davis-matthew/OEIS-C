/*
 * A031645  Numbers k such that the continued fraction for sqrt(k) has odd
 *          period and if the last term of the periodic part is deleted the
 *          two central terms are both 57.
 * https://oeis.org/A031645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k such that the simple continued
 *            fraction * for sqrt(k) has odd period, and after removing the
 *            last term of the period * the two middle terms of the remaining
 *            periodic part are both 57
 * Range:     u8 none, u16 n <= 2, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t lo = 0, hi = n;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid) hi = mid - 1;
        else lo = mid;
    }
    return lo;
}

static inline bool is_perfect_square(uint64_t n) {
    uint64_t r = isqrt_u64(n);
    return r * r == n;
}

/* Test whether k satisfies the required property. */
static bool is_candidate(uint64_t k) {
    if (is_perfect_square(k)) return false;

    uint64_t a0 = isqrt_u64(k);
    uint64_t m = 0, d = 1, a = a0;

    /* Period terms are stored in a small local array. */
    static uint64_t period[4096];
    size_t pos = 0;

    do {
        m = d * a - m;
        d = (k - m * m) / d;
        a = (a0 + m) / d;
        if (pos < 4096) period[pos++] = a;
    } while (a != 2 * a0);

    size_t L = pos;
    if (L < 3 || (L & 1) == 0) return false;

    size_t mid = (L - 1) >> 1;   /* mid index */
    if (period[mid - 1] == 57 && period[mid] == 57) return true;
    return false;
}

/* Cache for the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_k_checked = 1U;

static void ensure_cache(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!ptr) abort(); /* allocation failure */
    cache = ptr;
    cache_cap = new_cap;
}

/* Main function of the sequence. */
seq_t a(idx_t n) {
    if (n <= 0) return 0;  /* undefined for n <= 0: return 0 */

    if (n > cache_len) {
        ensure_cache((size_t)n);
        /* Generate terms until we have n. */
        uint64_t k = last_k_checked + 1;
        while (cache_len < (size_t)n) {
            if (is_candidate(k)) {
                cache[cache_len++] = (seq_t)k;
            }
            k++;
        }
        last_k_checked = k - 1;
    }
    return cache[n - 1];
}
