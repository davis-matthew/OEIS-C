/*
 * A201804  Numbers k such that 90*k + 11 is prime.
 * https://oeis.org/A201804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3078, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   free_seq, isPrimeGeneric, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n‑th non‐negative integer k such that 90*k + 11 is prime
 *  (OEIS A201804).  The routine builds the sequence on demand and
 *  remembers the values so that repeated calls in any order stay fast. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n) {
    /* static cache for the sequence values */
    static seq_t *seq = NULL;      /* array of computed k's */
    static size_t seq_cap = 0;     /* current allocation capacity */
    static size_t seq_len = 0;     /* number of elements stored */
    static idx_t cur_k = 0;        /* next k to test for primality */

    if (n == 0) return 0;          /* undefined for offset 1, safe fallback */

    /* create storage on first use */
    if (seq == NULL) {
        seq_cap = 16;
        seq = seq_alloc_new_array(seq_cap);
    }

    /* generate values until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* find the next k with 90*k + 11 prime */
        while (1) {
            /* candidate value to test for primality */
            seq_t val = (seq_t)(90 * cur_k + 11);

            if (isPrimeGeneric(val)) {
                /* enlarge storage if necessary */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap * 2;
                    seq_t *tmp = seq_alloc_new_array(new_cap);
                    for (size_t i = 0; i < seq_len; ++i)
                        tmp[i] = seq[i];
                    free_seq(seq);
                    seq = tmp;
                    seq_cap = new_cap;
                }
                /* store the found k */
                seq[seq_len++] = (seq_t)cur_k;
                cur_k++;
                break;          /* move on to next required term */
            }
            cur_k++;            /* try next k */
        }
    }

    /* return the requested term (1‑based indexing) */
    return seq[n - 1];
}
