/*
 * A073666  Rearrangement of natural numbers such that a(k)*a(k+1) + 1 is a
 *          prime for all k.
 * https://oeis.org/A073666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy construction:
   a(1) = 1; thereafter a(k) is the smallest positive integer not yet used
   such that a(k‑1) * a(k) + 1 is prime.
   The function memoises the sequence for arbitrary call order. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *perm   = NULL;      /* sequence values */
    static size_t perm_len = 0;       /* number of values computed */
    static size_t perm_cap = 0;       /* allocated capacity of perm */

    static uint8_t *used  = NULL;     /* 1 if number has been used */
    static size_t used_cap = 0;       /* allocated capacity of used */

    if (n == 0) return (seq_t)0;        /* offset is 1; behaviour for 0 is irrelevant */

    /* ensure perm has space for at least n elements */
    if (perm_len < (size_t)n) {
        size_t needed = (size_t)n;
        if (perm_cap < needed) {
            size_t new_cap = perm_cap ? perm_cap << 1 : 4;
            if (new_cap < needed) new_cap = needed;
            perm = (seq_t *)realloc(perm, new_cap * sizeof(seq_t));
            perm_cap = new_cap;
        }

        /* compute missing terms */
        for (size_t i = perm_len; i < (size_t)n; ++i) {
            seq_t val;

            if (i == 0) {                 /* first term */
                val = (seq_t)1;
                /* grow used array to at least index 1 */
                if (used_cap <= 1) {
                    size_t new_used = 2;
                    used = (uint8_t *)realloc(used, new_used);
                    for (size_t j = 0; j < new_used; ++j) used[j] = 0;
                    used_cap = new_used;
                }
                used[1] = 1;
            } else {
                seq_t prev = perm[i-1];
                seq_t chosen = 0;
                /* search for smallest unused k with prev*k+1 prime */
                for (uint64_t cand = 1;; ++cand) {
                    /* grow used table if needed */
                    if ((size_t)cand >= used_cap) {
                        size_t new_used = used_cap ? used_cap << 1 : 2;
                        if (new_used <= cand) new_used = cand + 1;
                        used = (uint8_t *)realloc(used, new_used);
                        for (size_t j = used_cap; j < new_used; ++j) used[j] = 0;
                        used_cap = new_used;
                    }
                    if (used[(size_t)cand]) continue;          /* already taken */
                    seq_t prod = prev * (seq_t)cand + (seq_t)1; /* prev*cand+1 */
                    if (isPrimeGeneric(prod)) {
                        chosen = (seq_t)cand;
                        used[(size_t)cand] = 1;
                        break;
                    }
                }
                val = chosen;
            }

            perm[i] = val;
        }
        perm_len = (size_t)n;
    }

    return perm[n-1];
}
