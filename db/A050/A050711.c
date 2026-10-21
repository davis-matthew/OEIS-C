/*
 * A050711  Inserting a digit '1' between adjacent digits of n makes a prime.
 * https://oeis.org/A050711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 174, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   insert_one, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A050711: the n-th integer whose decimal digits, after inserting
   the digit '1' between every pair of adjacent digits, form a prime.
   We generate terms by incremental search starting from 10 (the
   smallest candidate with two digits).
   The transformation is pure digit manipulation, and primality is
   tested with the provided generic primality routine. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/insert_one.h"

/* Cache of found terms so repeated calls are efficient. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Next candidate to test; start at the smallest two‑digit number. */
static seq_t cur_candidate = 10;

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n stored. */
    while (cache_len < (size_t)n) {
        seq_t cand = cur_candidate;
        seq_t trans = insert_one(cand);
        if (isPrimeGeneric(trans)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();      /* out of memory */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++cur_candidate;   /* next candidate */
    }
    return cache[(size_t)n - 1];
}
