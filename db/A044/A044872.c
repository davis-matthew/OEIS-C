/*
 * A044872  Positive integers having the same number of base-16 runs of odd
 *          length as even.
 * https://oeis.org/A044872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 450, u32 n <= 1180, u64 n <= 1180,
 *            u128 n <= 1180, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS A044872 sequence.
   a(n) : nth positive integer whose base‑16 representation contains
   the same number of runs of odd length as runs of even length.
   The function uses a simple bruteforce scan, caching generated
   terms across calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Counts runs of odd and even length in base‑16 representation of x.
   Returns true if the counts are equal. */
static bool has_equal_odd_even_runs(seq_t x)
{
    if (x == 0)   /* 0 has one run of odd length → no */
        return false;

    bool first = true;
    uint8_t prev_digit = 0;
    unsigned run_len = 0;
    unsigned odd_runs = 0, even_runs = 0;

    while (x > 0) {
        seq_t digit = x % 16;      /* extract least significant hex digit */
        x /= 16;
        uint8_t d = (uint8_t)digit;

        if (first) {
            first = false;
            prev_digit = d;
            run_len = 1;
        } else if (d == prev_digit) {
            run_len++;
        } else {
            /* finish previous run */
            if (run_len & 1)
                odd_runs++;
            else
                even_runs++;
            prev_digit = d;
            run_len = 1;
        }
    }

    /* finalize last run */
    if (run_len & 1)
        odd_runs++;
    else
        even_runs++;

    return odd_runs == even_runs;
}

/* Cache dynamic array of pre‑computed terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* target index n is 1‑based. */
    if (n == 0)
        return 0;   /* undefined, but guard anyway */

    size_t target = (size_t)n;

    /* Grow cache if needed */
    if (target > cache_size) {
        size_t new_cap = target;
        seq_t *new_cache = NULL;
        if (cache == NULL) {
            new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (new_cache == NULL) {
            /* allocation failure – return 0 as error sentinel */
            return 0;
        }
        cache = new_cache;

        /* determine next candidate number */
        seq_t next_candidate;
        if (cache_size == 0) {
            next_candidate = 256;          /* first qualifying number */
        } else {
            next_candidate = cache[cache_size - 1] + 1;
        }

        /* Generate terms until reaching target */
        for (size_t i = cache_size; i < target; ++i) {
            while (!has_equal_odd_even_runs(next_candidate)) {
                next_candidate++;
            }
            cache[i] = next_candidate;
            next_candidate++;      /* next search starts from one after found */
        }
        cache_size = target;
    }

    return cache[n - 1];
}
