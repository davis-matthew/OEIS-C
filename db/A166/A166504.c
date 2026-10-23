/*
 * A166504  Slime numbers: numbers which are the concatenation of primes, with
 *          "leading zeros" allowed.
 * https://oeis.org/A166504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   compare_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A166504: slime numbers; numbers that are prime or can be
   written as a concatenation of two earlier slime numbers, with the
   second part possibly having leading zeros.  Computed by incremental
   search and memoization. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/compare_seq_t.h"

/* Global storage for the generated slime numbers. */
static seq_t *slime_seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static seq_t last_checked = (seq_t)2;
static bool global_init = false;

/* Ensure the array has enough capacity for at least new_cap elements. */
static void ensure_seq_cap(size_t new_cap) {
    if (seq_cap >= new_cap) return;
    size_t cap = seq_cap ? seq_cap * 2 : 16;
    while (cap < new_cap) cap *= 2;
    seq_t *tmp = (seq_t *)realloc(slime_seq, cap * sizeof(seq_t));
    if (!tmp) {
        free(slime_seq);
        exit(1);
    }
    slime_seq = tmp;
    seq_cap   = cap;
}

/* Append a new slime number; the array is kept unsorted for simplicity.
   (Binary search works on the sorted copy created on demand.) */
static void add_slime(seq_t val) {
    ensure_seq_cap(seq_len + 1);
    slime_seq[seq_len++] = val;
}

/* Binary search for val in the sorted slime_seq array. */
static bool binary_search_slime(seq_t val) {
    size_t low = 0, high = seq_len;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        int cmp = compare_seq_t(&slime_seq[mid], &val);
        if (cmp < 0)
            low = mid + 1;
        else if (cmp > 0)
            high = mid;
        else
            return true;
    }
    return false;
}

/* One‑time global initialization. */
static void init_once(void) {
    if (global_init) return;
    global_init = true;
    seq_len = 0;
    seq_cap = 0;
    slime_seq = NULL;
    last_checked = (seq_t)2;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */
    init_once();

    /* Generate slime numbers until we have at least n terms. */
    while (seq_len < (size_t)n) {
        seq_t cur = last_checked++;
        /* Prime numbers are automatically slime. */
        if (is_prime_big(cur)) {
            add_slime(cur);
            continue;
        }
        /* Check all possible splits with the right part possibly
           having leading zeros. */
        seq_t divisor = (seq_t)10;
        bool ok = false;
        while (divisor <= cur) {
            seq_t left  = cur / divisor;
            seq_t right = cur % divisor;
            if (binary_search_slime(left) && binary_search_slime(right)) {
                ok = true;
                break;
            }
            divisor *= (seq_t)10;
        }
        if (ok) add_slime(cur);
    }

    return slime_seq[n - 1];
}
