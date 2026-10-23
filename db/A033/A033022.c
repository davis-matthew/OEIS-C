/*
 * A033022  Numbers whose base-9 expansion has no run of digits with length <
 *          2.
 * https://oeis.org/A033022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 224, u32 n <= 1100, u64 n <= 1100,
 *            u128 n <= 1100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033022: Numbers whose base‑9 expansion has no run of digits with length < 2. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Check whether a number has all digit runs in base 9 of length >= 2. */
static inline bool is_valid(seq_t x)
{
    int run_len = 0;
    int prev_digit = -1;          /* Sentinel for "no previous digit". */

    /* Scan the base‑9 digits from least significant to most. */
    while (x > 0) {
        int digit = (int)(x % ((seq_t)9));
        if (digit == prev_digit) {
            run_len++;
        } else {
            /* We just finished a run.  It must have length ≥ 2. */
            if (run_len == 1)
                return false;
            run_len = 1;
            prev_digit = digit;
        }
        x /= ((seq_t)9);
    }

    /* The final run must also have length ≥ 2. */
    return run_len != 1;  /* run_len can be 0 only when x was 0 (not used). */
}

#ifdef __cplusplus
/* ------- C++ version (uses std::vector) ------- */
static std::vector<seq_t> cache;
static seq_t next_candidate = (seq_t)10;   /* First possible candidate. */

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (cache.size() < n) {
        /* Skip candidates that do not satisfy the property. */
        while (!is_valid(next_candidate))
            next_candidate++;

        cache.push_back(next_candidate);
        next_candidate++;
    }
    return cache[n - 1];
}

#else
/* ------- C version (uses malloc/realloc) ------- */
static seq_t *cache = NULL;
static size_t cache_size   = 0;
static size_t cache_capacity = 0;
static seq_t next_candidate = (seq_t)10;   /* First possible candidate. */

seq_t a(idx_t n)
{
    /* Ensure the cache holds at least n entries. */
    while (cache_size < (size_t)n) {
        /* Find next valid candidate. */
        while (!is_valid(next_candidate))
            next_candidate++;

        /* Grow the cache if necessary. */
        if (cache_size >= cache_capacity) {
            size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
            seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_capacity = new_cap;
        }

        cache[cache_size++] = next_candidate;
        next_candidate++;
    }
    return cache[n - 1];
}
#endif
