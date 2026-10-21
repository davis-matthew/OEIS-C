/*
 * A065216  Numbers which need eleven 'Reverse and Add' steps to reach a
 *          palindrome.
 * https://oeis.org/A065216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 177, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   reverse_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A065216
   a(n) = the n-th integer k such that the Reverse‑and‑Add process
   takes exactly 11 steps to reach a palindrome.
   Algorithm:
   • Iterate over natural numbers.
   • For each non‑palindromic candidate, apply at most 11 reverse‑and‑add steps.
   • If the first palindrome appears after exactly 11 steps, store it.
   • Cache all found values so that subsequent calls are fast and order‑independent.
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_seq.h"

static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static idx_t cached_count = 0;
static idx_t last_checked = 1;

static inline bool is_palindrome(seq_t x) {
    return x == reverse_seq(x);
}

static void ensure_capacity(size_t needed) {
    if (needed <= cache_capacity) return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *newbuf = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache != NULL) {
        for (size_t i = 0; i < cached_count; ++i)
            newbuf[i] = cache[i];
        free(cache);
    }
    cache = newbuf;
    cache_capacity = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                       /* undefined but harmless */
    if (cached_count >= n) return cache[n - 1]; /* already computed */

    for (;;) {
        seq_t cand = last_checked++;
        if (is_palindrome(cand))
            continue; /* step count 0, skip */

        seq_t cur = cand;
        int steps = 0;
        bool early = false;

        while (steps < 11) {
            if (is_palindrome(cur)) { early = true; break; }
            cur += reverse_seq(cur);
            ++steps;
        }

        if (!early && steps == 11 && is_palindrome(cur)) {
            ensure_capacity(cached_count + 1);
            cache[cached_count++] = cand;
            if (cached_count == n)
                return cand;
        }
    }
}
