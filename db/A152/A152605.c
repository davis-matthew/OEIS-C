/*
 * A152605  a(1) = 1; thereafter a(n) is always the smallest integer > a(n-1)
 *          not leading to a contradiction, such that any five consecutive
 *          digits in the sequence sum up to a prime.
 * https://oeis.org/A152605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 1; for n > 1, a(n) is the smallest integer > a(n−1) such
 *            that when the decimal representations of a(1)…a(n) are
 *            concatenated, every window of five consecutive digits has a
 *            prime sum
 * Range:     u8 n <= 11, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Convert a value to its decimal digits (least significant first).  */
/* ------------------------------------------------------------------ */
static void seq_digits(seq_t v, uint8_t *buf, size_t *len)
{
    if (v == (seq_t)0) {
        *len = 1;
        buf[0] = 0;
        return;
    }
    size_t i = 0;
    while (v != (seq_t)0) {
        buf[i++] = (uint8_t)(v % (seq_t)10);
        v /= (seq_t)10;
    }
    *len = i;
}

/* ------------------------------------------------------------------ */
/* Pre‑computed primality of sums of 5 decimal digits (0 … 45).        */
/* ------------------------------------------------------------------ */
static const bool prime[46] = {
    /* 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
       21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40
       41 42 43 44 45 */
    false, false, true, true, false, true, false, true, false, false, false,
    true, false, true, false, false, false, true, false, true, false,
    false, false, true, false, false, false, false, false, true, false,
    true, false, false, false, false, false, true, false, false, false,
    true, false, true, false, false
};

/* ------------------------------------------------------------------ */
/* Cache for already computed terms.                                  */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;      /* Stores a(1)…a(k) */
static size_t cache_len = 0;     /* Number of terms stored   */
static size_t cache_cap = 0;     /* Capacity of cache array  */

/* Ensure the cache can hold at least 'need' terms. */
static void ensure_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
    if (new_cap < need) new_cap = need;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_cache == NULL) exit(EXIT_FAILURE);
    if (cache != NULL) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Sliding‑window state for the concatenated digit stream.             */
/* ------------------------------------------------------------------ */
static int last[5];             /* Most recent digits; last[0] is newest */
static size_t last_count;       /* Total digits processed so far       */
static int last_sum;            /* Sum of the current window (only valid when last_count>=5) */

/* ------------------------------------------------------------------ */
/* Test whether appending 'cand' keeps all 5‑digit windows prime‑sum.   */
/* If successful, update global state and return true; otherwise leave   */
/* state unchanged and return false.                                    */
/* ------------------------------------------------------------------ */
static bool test_candidate(seq_t cand)
{
    /* Local copies to simulate appending. */
    int temp_last[5];
    memcpy(temp_last, last, 5 * sizeof(int));
    size_t temp_count = last_count;
    int temp_sum = last_sum;

    /* Get digits of candidate (least significant first). */
    uint8_t buf[128];
    size_t len;
    seq_digits(cand, buf, &len);

    /* Process digits from most significant to least. */
    for (int idx = (int)len - 1; idx >= 0; idx--) {
        int d = buf[idx];
        if (temp_count < 5) {          /* window not yet full              */
            temp_last[temp_count] = d;
            temp_sum += d;
            temp_count++;
            if (temp_count == 5 && !prime[temp_sum]) return false;
        } else {                       /* window already full              */
            int out = temp_last[temp_count % 5];
            temp_sum += d - out;
            temp_last[temp_count % 5] = d;
            temp_count++;
            if (!prime[temp_sum]) return false;
        }
    }

    /* All windows valid: commit state. */
    memcpy(last, temp_last, 5 * sizeof(int));
    last_count = temp_count;
    last_sum = temp_sum;
    return true;
}

/* ------------------------------------------------------------------ */
/* Required function: compute a(n).                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    /* First call initialisation. */
    if (cache_len == 0) {
        ensure_cache(32);
        cache[0] = (seq_t)1;             /* a(1) = 1 */
        cache_len = 1;

        /* Initialise sliding window with the single digit '1'. */
        last[0] = 1;
        last_count = 1;
        last_sum = 1;                    /* meaningful after 5 digits seen */
    }

    /* Build terms up to n if needed. */
    while (cache_len < (size_t)n) {
        seq_t prev = cache[cache_len - 1];
        seq_t cand = prev + (seq_t)1;

        /* Find the smallest candidate that keeps the property. */
        while (!test_candidate(cand)) cand = cand + (seq_t)1;

        ensure_cache(cache_len + 1);
        cache[cache_len] = cand;
        cache_len++;
    }

    return cache[n - 1];
}
