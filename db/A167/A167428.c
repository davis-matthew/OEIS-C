/*
 * A167428  Smallest non-isolated (or twin) prime > n-th non-isolated
 *          nonprime.
 * https://oeis.org/A167428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest non‑isolated prime > the n‑th non‑isolated
 *            nonprime
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

/* Append a value to a dynamic array of seq_t. */
static void push_seq(seq_t **arr, size_t *size, size_t *cap, seq_t val) {
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

/* Smallest non‑isolated prime greater than a given threshold. */
static seq_t smallest_nonisolated_prime_gt(seq_t threshold) {
    seq_t cand = threshold + 1;
    while (1) {
        uint64_t u = (uint64_t)cand;
        if (is_prime_small(u)) {
            bool twin = false;
            if (u >= 2) twin |= is_prime_small(u - 2);
            twin |= is_prime_small(u + 2);
            if (twin) return cand;
        }
        cand++;
    }
}

/* Global state for memoisation. */
static seq_t *iso_np = NULL;      /* Non‑isolated non‑primes */
static size_t iso_np_size = 0;
static size_t iso_np_cap = 0;

static seq_t *answers = NULL;     /* Cached sequence a(n) */
static size_t ans_size = 0;
static size_t ans_cap = 0;

static idx_t current_x = 0;   /* Next integer to examine for iso_np */

/* Generate iso_np until we have at least n elements. */
static void ensure_iso_np(idx_t n) {
    while (iso_np_size < n) {
        uint64_t x = (uint64_t)current_x;
        bool prime = is_prime_small(x);
        if (!prime) {
            bool left_np  = false;
            bool right_np = false;
            if (current_x > 0) left_np  = !is_prime_small(x - 1);
            right_np = !is_prime_small(x + 1);
            if (left_np || right_np) {
                push_seq(&iso_np, &iso_np_size, &iso_np_cap, (seq_t)current_x);
            }
        }
        current_x++;
    }
}

/* Generate answers until we have at least n elements. */
static void ensure_answers(idx_t n) {
    if (ans_size >= n) return;
    ensure_iso_np(n);
    for (size_t i = ans_size; i < (size_t)n; ++i) {
        seq_t threshold = iso_np[i];
        seq_t val = smallest_nonisolated_prime_gt(threshold);
        push_seq(&answers, &ans_size, &ans_cap, val);
    }
}

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1.  For n < 1 the behaviour is undefined. */
    if (n == 0) return 0;
    ensure_answers(n);
    return answers[n-1];
}
