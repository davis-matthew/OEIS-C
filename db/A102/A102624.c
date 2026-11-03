/*
 * A102624  Numbers k such that the number k23 is prime.
 * https://oeis.org/A102624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 971 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer k such that k23 (i.e., k*100+23) is prime. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    static seq_t *terms    = NULL;
    static size_t  cap     = 0;
    static size_t  len     = 0;
    static uint64_t next_k = 0;

    if (!terms) {
        cap = 64;
        terms = mem_new(seq_t, cap);
        if (!terms) return (seq_t)0;
    }

    while (len < n) {
        uint64_t cand = next_k * 100ULL + 23ULL;
        if (is_prime_u64(cand)) {
            if (len + 1 > cap) {
                size_t newcap = cap * 2;
                seq_t *tmp = mem_resize(seq_t, terms, newcap);
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
