/*
 * A106773  Primes with digit sum = 40.
 * https://oeis.org/A106773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 49 terms through n = 975 (2026-11-03)
 * Helpers:   digit_sum_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth prime with digit sum 40 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sum_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

seq_t a(idx_t n) {
    static idx_t count = 0;            /* primes found so far */
    static seq_t *primes = NULL;       /* dynamic array storing them */
    static size_t cap = 0;             /* allocated capacity of the array */
    static uint64_t next_candidate = 2;/* next integer to test for primality */

    /* Initialize the array on first call */
    if (!primes) {
        cap = 16;
        primes = mem_new(seq_t, cap);
    }

    /* Generate primes until we have enough */
    while (count < n) {
        uint64_t cand = next_candidate;

        if (is_prime_u64(cand) && digit_sum_u64(cand) == 40) {
            if (count >= cap) {
                size_t newcap = cap * 2;
                seq_t *tmp = mem_resize(seq_t, primes, newcap);
                if (!tmp) return 0; /* allocation failure, unlikely */
                primes = tmp;
                cap = newcap;
            }
            primes[count] = (seq_t)cand;
            ++count;
        }

        /* Advance to the next candidate: skip even numbers after 2 */
        if (cand <= 2) {
            next_candidate = 3;
        } else {
            next_candidate = cand + 2;
        }
    }

    return primes[n - 1];
}
