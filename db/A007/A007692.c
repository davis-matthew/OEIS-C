/*
 * A007692  Numbers that are the sum of 2 nonzero squares in 2 or more ways.
 * https://oeis.org/A007692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Search for n-th number that can be represented as sum of 2
 *            nonzero squares in 2+ ways
 * Range:     u8 n <= 12, u16 n <= 7014, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/a000037_isqrt.h"


static int count_representations(uint64_t k) {
    int count = 0;
    
    for (uint64_t a = 1; 2 * a * a <= k; a++) {
        uint64_t b_sq = k - a * a;
        uint64_t b = a000037_isqrt(b_sq);
        
        if (b >= a && b * b == b_sq) {
            count++;
        }
    }
    
    return count;
}

seq_t a(idx_t n) {
    static seq_t *terms = NULL;
    static idx_t computed = 0;
    static idx_t capacity = 0;
    static uint64_t next_candidate = 2;
    
    if (n <= computed) {
        return terms[n - 1];
    }
    
    if (terms == NULL) {
        capacity = 128;
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
        computed = 0;
        next_candidate = 2;
    }
    
    while (computed < n) {
        if (count_representations(next_candidate) >= 2) {
            if (computed >= capacity) {
                capacity *= 2;
                seq_t *new_terms = (seq_t *)malloc(capacity * sizeof(seq_t));
                for (idx_t i = 0; i < computed; i++) {
                    new_terms[i] = terms[i];
                }
                free(terms);
                terms = new_terms;
            }
            terms[computed] = (seq_t)next_candidate;
            computed++;
        }
        next_candidate++;
    }
    
    return terms[n - 1];
}
