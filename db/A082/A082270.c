/*
 * A082270  a(1) = 1; for n >= 1, a(n+1) = smallest number not occurring
 *          earlier such that a(n)*a(n+1) + 1 is a palindrome.
 * https://oeis.org/A082270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_seen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(1) = 1; for n >= 1,
   a(n+1) = smallest number not occurring earlier such that
   a(n)*a(n+1) + 1 is a palindrome. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_seen.h"

/* ---------- helpers ---------- */

static bool is_palindrome(seq_t val)
{
    if (val == (seq_t)0) return true;
    /* store digits in reverse order */
    static unsigned char digits[8192];      /* enough for all needed numbers */
    size_t len = 0;
    seq_t tmp = val;
    while (tmp != (seq_t)0) {
        digits[len++] = (unsigned char)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }
    for (size_t i = 0; i < len / 2; ++i)
        if (digits[i] != digits[len - 1 - i]) return false;
    return true;
}

/* ---------- dynamic arrays ---------- */

static seq_t *cache = NULL;      /* sequence terms */
static size_t cache_size = 0;
static size_t cache_capacity = 0;

static seq_t *seen = NULL;       /* all previous terms for lookup */
static size_t seen_size = 0;
static size_t seen_capacity = 0;

static void grow_cache(void)
{
    size_t new_cap = (cache_capacity == 0) ? 1 : cache_capacity * 2;
    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!ptr) { abort(); }
    cache = ptr;
    cache_capacity = new_cap;
}

static void grow_seen(void)
{
    size_t new_cap = (seen_capacity == 0) ? 1 : seen_capacity * 2;
    seq_t *ptr = (seq_t *)realloc(seen, new_cap * sizeof(seq_t));
    if (!ptr) { abort(); }
    seen = ptr;
    seen_capacity = new_cap;
}

/* ---------- ensure terms up to n are computed ---------- */

static void ensure_computed(idx_t n)
{
    if (n == 0) return;
    if (cache_size >= (size_t)n) return;

    /* initialize first term on first call */
    if (cache_size == 0) {
        grow_cache();  /* capacity 1 */
        cache[0] = (seq_t)1;
        cache_size = 1;
        grow_seen();
        seen[0] = (seq_t)1;
        seen_size = 1;
    }

    while (cache_size < (size_t)n) {
        seq_t prev = cache[cache_size - 1];
        seq_t cand = (seq_t)1;

        while (1) {
            /* skip numbers already in the sequence */
            if (!is_seen(seen, seen_size, cand)) {
                seq_t prod = prev * cand + (seq_t)1;
                if (is_palindrome(prod)) {
                    /* append new term */
                    if (cache_size == cache_capacity) grow_cache();
                    cache[cache_size++] = cand;
                    if (seen_size == seen_capacity) grow_seen();
                    seen[seen_size++] = cand;
                    break;
                }
            }
            ++cand;
        }
    }
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1 */
    ensure_computed(n);
    return cache[n - 1];              /* 1‑based indexing */
}
