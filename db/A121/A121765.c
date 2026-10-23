/*
 * A121765  Numbers k such that 6*k-1 is composite while 6*k+1 is prime.
 * https://oeis.org/A121765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 2949, u32 n <= 1021, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n‑th integer k such that 6*k‑1 is composite
   while 6*k+1 is prime. */
/* The function builds the sequence incrementally; memoisation allows
   random access to any n ≥ 1 in O(1) after the needed terms have been
   generated. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The sequence is 1‑based; n==0 should never be used. */
    if (n == 0)
        return 0;

    /* Static state for lazy, incremental construction. */
    static seq_t *seq      = NULL;   /* Array of found terms. */
    static size_t seq_len  = 0;      /* Number of terms stored. */
    static size_t seq_cap  = 0;      /* Current capacity of the array. */
    static uint64_t next_k = 1;      /* Next candidate k to test. */

    /* Initialise the array lazily. */
    if (seq_cap == 0) {
        seq_cap = 64;                /* Initial capacity. */
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        /* Use seq_t for arithmetic; this works for all supported types. */
        seq_t k_val = (seq_t)next_k;
        seq_t v1 = (seq_t)6 * k_val - (seq_t)1;  /* 6*k-1 */
        seq_t v2 = (seq_t)6 * k_val + (seq_t)1;  /* 6*k+1 */

        if (!is_prime_big(v1) && is_prime_big(v2)) {
            /* Ensure there is room for one more element. */
            if (seq_len == seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len] = k_val;
            seq_len++;
        }
        next_k++;
    }

    /* Return the stored term (offset 1). */
    return seq[n - 1];
}
