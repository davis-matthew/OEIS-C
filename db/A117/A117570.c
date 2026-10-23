/*
 * A117570  Numbers of the form k * (sum of digits of k) listed sorted with
 *          multiplicity.
 * https://oeis.org/A117570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, merge_sorted
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A117570: the sorted multiset of k * sum_of_digits(k). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"          /* seq_t digit_sum(seq_t x); */
#include "../utils/merge_sorted.h"

/* Global storage for the accumulated sorted values */
static seq_t *sorted_vals = NULL;
static size_t sorted_len   = 0;
static uint64_t max_k_generated = 0;   /* largest k that has been processed */

/* Batch size for each expansion. */
#define BATCH 100000U

/* Comparator for qsort when sorting seq_t values. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t av = *(const seq_t *)a;
    seq_t bv = *(const seq_t *)b;
    return (av < bv) ? -1 : (av > bv) ? 1 : 0;
}

/* Ensure that the global storage contains at least n terms and that every value
   ≤ a(n) has been generated from some k.*/
static void ensure_terms(idx_t n)
{
    /* Initial population if this is the first call. */
    if (sorted_vals == NULL) {
        size_t temp_len = BATCH;
        seq_t *temp = (seq_t *)malloc(temp_len * sizeof(seq_t));
        for (uint64_t k = 1; k <= BATCH; ++k) {
            temp[k - 1] = (seq_t)k * digit_sum((seq_t)k);
        }
        qsort(temp, temp_len, sizeof(seq_t), cmp_seq);

        /* Reserve space for 0 as the first term. */
        sorted_vals = (seq_t *)malloc((temp_len + 1) * sizeof(seq_t));
        sorted_vals[0] = (seq_t)0;                /* a(1) = 0 */
        for (size_t i = 0; i < temp_len; ++i) {
            sorted_vals[i + 1] = temp[i];
        }
        free(temp);

        sorted_len = temp_len + 1;
        max_k_generated = BATCH;
    }

    /* Keep generating batches until we can safely answer the request. */
    while (true) {
        /* If we already have n terms and all k up to the candidate have been
           processed then we are finished. */
        if (sorted_len >= (size_t)n) {
            seq_t candidate = sorted_vals[(size_t)n - 1];
            uint64_t cand_u64 = (uint64_t)candidate; /* safe for the sizes we need */
            if (max_k_generated >= cand_u64) {
                return;
            }
        }

        /* Generate next batch of k values. */
        size_t new_k_start = max_k_generated + 1;
        size_t new_k_end   = max_k_generated + BATCH;
        seq_t *new_vals = (seq_t *)malloc(BATCH * sizeof(seq_t));

        for (uint64_t k = new_k_start; k <= new_k_end; ++k) {
            new_vals[k - new_k_start] = (seq_t)k * digit_sum((seq_t)k);
        }

        qsort(new_vals, BATCH, sizeof(seq_t), cmp_seq);

        /* Merge with existing sorted values. */
        seq_t *merged = merge_sorted(sorted_vals, sorted_len,
                                     new_vals, BATCH);

        free(sorted_vals);
        free(new_vals);

        sorted_vals = merged;
        sorted_len += BATCH;
        max_k_generated = new_k_end;
    }
}

/* Public interface: return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0 as per problem statement; return 0 for safety. */
        return (seq_t)0;
    }
    ensure_terms(n);
    return sorted_vals[(size_t)n - 1];
}
