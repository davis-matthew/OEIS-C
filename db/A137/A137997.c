/*
 * A137997  Numbers which are not the sum of a triangular number and zero or a
 *          prime = 3 (mod 4).
 * https://oeis.org/A137997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137997: numbers that cannot be written as
 * p + T_k where p is 0 or a prime congruent to 3 (mod 4),
 * and T_k = k(k+1)/2 is the k-th triangular number (k ≥ 0).
 *
 * The implementation stores all already found terms in a
 * static dynamic array.  For each new candidate integer x,
 * it tries every triangular number T_k ≤ x.  If x – T_k equals
 * 0, or equals a prime ≡ 3 (mod 4), the candidate is excluded.
 * Otherwise x is appended to the cache.  The list is built up
 * lazily until the requested index n is reached.
 *
 * This algorithm runs in O(n·√m) time, where n is the required
 * index and m the n‑th term (≈ 10⁵ for n ≈ 10⁴).  It easily meets
 * the 2.0‑second per‑call requirement for the typical builds.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"

/* external types: seq_t and idx_t are provided by the build environment */
extern seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *terms    = NULL;   /* array of terms */
    static size_t terms_cnt = 0;     /* number of terms stored */
    static size_t terms_cap = 0;     /* current capacity of the array */
    static idx_t  candidate  = 2;    /* next integer to test */

    /* initialise cache on first call */
    if (terms == NULL) {
        terms_cap = 32;
        terms     = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (terms == NULL) {
            /* out of memory; caller will see an error elsewhere */
            exit(1);
        }
    }

    /* grow array when necessary */
    if (terms_cnt >= terms_cap) {
        size_t new_cap = terms_cap * 2;
        seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (new_terms == NULL) {
            exit(1);
        }
        terms = new_terms;
        terms_cap = new_cap;
    }

    /* build list until we have at least n terms */
    while (terms_cnt < (size_t)n) {
        bool representable = false;
        seq_t T = 0;           /* current triangular number */
        idx_t k = 0;           /* index for triangular number */
        seq_t cand = (seq_t)candidate;

        while (T <= cand) {
            seq_t rem = cand - T;
            if (rem == (seq_t)0) {
                representable = true;
                break;
            }
            if (is_prime_big(rem) && ((rem % 4u) == 3u)) {
                representable = true;
                break;
            }
            k++;
            T += (seq_t)k;   /* next triangular number */
        }

        if (!representable) {
            if (terms_cnt >= terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (new_terms == NULL) {
                    exit(1);
                }
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = cand;
        }
        candidate++;
    }

    return terms[n - 1];
}
