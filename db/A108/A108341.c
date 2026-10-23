/*
 * A108341  Numbers n such that 997*n - 1009 is prime.
 * https://oeis.org/A108341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108341:  Numbers n such that 997*n – 1009 is prime.
   Implementation uses an incremental search.  Results are cached
   across calls so that any order of queries up to N works in
   O(N) time. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache for already found terms */
    static seq_t *terms = NULL;
    static size_t size = 0;       /* number of terms stored */
    static size_t cap  = 0;       /* capacity of the allocated array */
    static uint64_t last_checked = 0;   /* last candidate examined */

    /* fill cache until it contains n terms */
    while (size < (size_t)n) {
        uint64_t candidate = last_checked + 1;
        for (;;) {
            uint64_t test_val = 997ULL * candidate - 1009ULL;
            if (is_prime(test_val)) {
                /* grow the cache if needed */
                if (size == cap) {
                    size_t newcap = (cap ? cap * 2 : 16);
                    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                    if (!tmp) abort();
                    terms = tmp;
                    cap = newcap;
                }
                terms[size++] = (seq_t)candidate;
                break;           /* found next term */
            }
            ++candidate;        /* try next n */
        }
        last_checked = candidate;
    }

    return terms[(size_t)n - 1];
}
