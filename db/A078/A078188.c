/*
 * A078188  a(1) = 1; for n > 1 take the digits of a(n-1) one after the other
 *          and remove one occurrence of this digit from the digits of m = k*n
 *          (k > 0) if possible; a(n) is the smallest multiple m of n which
 *          has the least number of remaining digits (counted with
 *          multiplicity).
 * https://oeis.org/A078188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A078188
   a(1) = 1.
   For n > 1 let p = a(n‑1).  For each multiple m = k*n (k>0) write the
   decimal digits of p and remove from m one occurrence of each of those
   digits (if present).  The number of digits that remain is counted.
   a(n) is the smallest multiple of n that yields the fewest remaining
   digits; ties are broken in favour of the smaller multiple.
   This implementation evaluates multiples incrementally, stops once
   no further multiple can beat the current best, and memoises results
   so that calls may be made in any order.  */

/* ----------  Standard headers  ---------- */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

/* ----------  Helper routines  ---------- */

/* Compute decimal digit frequencies of x into freq[10] */
static void digit_freq(seq_t x, unsigned char freq[10]) {
    unsigned char tmp[10] = {0};
    if (x == (seq_t)0) {
        tmp[0] = 1;
    } else {
        while (x > (seq_t)0) {
            unsigned d = (unsigned)(x % (seq_t)10);
            tmp[d]++;
            x /= (seq_t)10;
        }
    }
    for (int i = 0; i < 10; ++i) freq[i] = tmp[i];
}

/* Return length of decimal representation of x */
static int digit_len(seq_t x) {
    int len = 0;
    if (x == (seq_t)0) return 1;
    while (x > (seq_t)0) {
        ++len;
        x /= (seq_t)10;
    }
    return len;
}

/* ----------  Memoisation cache  ---------- */

static seq_t *cache = NULL;          /* array of a(1), a(2), …  */
static size_t cache_cap = 0;          /* capacity in terms of index+1 */
static idx_t last_computed = 0;       /* largest n already computed */

/* Grow cache to accommodate at least index n (n+1 elements) */
static void ensure_cache(idx_t n) {
    if ((size_t)n < cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 32 : cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* reallocation failure – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* ----------  Sequence function  ---------- */

seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0;    /* offset is 1 */

    ensure_cache(n + (idx_t)1);           /* need index n */

    for (idx_t i = last_computed + (idx_t)1; i <= n; ++i) {
        if (i == (idx_t)1) {              /* base case */
            cache[1] = (seq_t)1;
            last_computed = 1;
            continue;
        }

        seq_t prev = cache[i - (idx_t)1];
        unsigned char freq_prev[10];
        digit_freq(prev, freq_prev);
        int len_prev = digit_len(prev);

        seq_t best_m = (seq_t)0;
        int best_remaining = INT_MAX;
        bool best_set = false;

        for (idx_t k = (idx_t)1; ; ++k) {
            seq_t m = (seq_t)i * (seq_t)k;

            /* digit statistics for m */
            unsigned char freq_m[10];
            digit_freq(m, freq_m);

            int matched = 0;
            for (int d = 0; d < 10; ++d) {
                matched += (freq_prev[d] < freq_m[d]) ? freq_prev[d] : freq_m[d];
            }

            int len_m = digit_len(m);
            int remaining = len_m - matched;

            if (!best_set ||
                remaining < best_remaining ||
                (remaining == best_remaining && m < best_m)) {
                best_m = m;
                best_remaining = remaining;
                best_set = true;
                if (best_remaining == 0) break;   /* cannot beat zero */
            }

            /* Lower bound for any further multiple:
               remaining >= len_m - len_prev */
            int lower_bound = len_m - len_prev;
            if (lower_bound > best_remaining) break;
        }

        cache[i] = best_m;
        last_computed = i;
    }

    return cache[n];
}
