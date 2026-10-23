/*
 * A102624  Numbers k such that the number k23 is prime.
 * https://oeis.org/A102624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer k such that k23 (i.e., k*100+23) is prime. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n) {
    static seq_t *terms    = NULL;
    static size_t  cap     = 0;
    static size_t  len     = 0;
    static uint64_t next_k = 0;

    if (!terms) {
        cap = 64;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) return (seq_t)0;
    }

    while (len < n) {
        uint64_t cand = next_k * 100ULL + 23ULL;
        if (is_prime_uint64(cand)) {
            if (len + 1 > cap) {
                size_t newcap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;
                terms = tmp;
                cap   = newcap;
            }
            terms[len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return terms[n - 1];
}
