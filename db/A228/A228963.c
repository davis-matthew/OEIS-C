/*
 * A228963  Smallest sets of 6 consecutive abundant numbers in arithmetic
 *          progression. The initial abundant number is listed.
 * https://oeis.org/A228963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 159, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) for OEIS A228963: the first element of the nth
   set of six consecutive abundant numbers that form an arithmetic progression.
   It precomputes the sums of proper divisors up to a fixed limit, detects all
   such progressions, stores the starting numbers, and returns the requested
   value. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Occupy enough space to find at least the first 10000 terms. */
#define MAX_LIMIT     16000000U      /* 16 million → ~64 MB of uint32_t */

/* Keep the computed results around for repeated calls. */
static bool initialized = false;
static seq_t *results = NULL;
static size_t res_cap = 0, res_cnt = 0;

/* Helper to grow an array of seq_t. */
static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 128 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Main function requested by the test harness. */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* Allocate array for sums of proper divisors. */
        uint32_t *divsum = (uint32_t *)calloc((size_t)(MAX_LIMIT + 1), sizeof(uint32_t));
        if (!divsum) exit(1);   /* impossible, but keep compiler happy */

        /* Build divisor-sum array using a simple sieve. */
        for (idx_t i = 1; i <= MAX_LIMIT / 2; ++i) {
            for (idx_t j = i << 1; j <= MAX_LIMIT; j += i) {
                divsum[j] += (uint32_t)i;
            }
        }

        /* Slide through the numbers collecting windows of 6 consecutive
           abundant numbers that are in arithmetic progression. */
        seq_t last[6];
        size_t len = 0;

        for (idx_t num = 2; num <= MAX_LIMIT; ++num) {
            if (divsum[num] > num) {
                /* An abundant number. */
                if (len < 6) {
                    last[len++] = (seq_t)num;
                } else {
                    /* Shift left by one and append the new element. */
                    for (size_t k = 0; k < 5; ++k) last[k] = last[k + 1];
                    last[5] = (seq_t)num;

                    /* Check whether the six numbers form an AP. */
                    seq_t d = last[5] - last[4];
                    if (last[4] - last[3] == d &&
                        last[3] - last[2] == d &&
                        last[2] - last[1] == d &&
                        last[1] - last[0] == d) {

                        grow_seq_array(&results, &res_cap, res_cnt + 1);
                        results[res_cnt++] = last[0];
                    }
                }
            }
        }

        free(divsum);
        initialized = true;
    }

    /* The sequence is indexed from 1. */
    return results[n - 1];
}
