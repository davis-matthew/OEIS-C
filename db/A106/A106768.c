/*
 * A106768  Primes with digit sum = 32.
 * https://oeis.org/A106768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 131, u32 n <= 6200, u64 n <= 6200,
 *            u128 n <= 6200, big any n
 * Helpers:   digit_sum_uint64, is_prime, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes with digit sum 32.  Computed by search. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* bool is_prime(uint64_t n); */
#include "../utils/digit_sum_uint64.h"  /* uint16_t digit_sum_uint64(uint64_t x); */
#include "../utils/seq_malloc.h"        /* seq_t *seq_malloc(idx_t n); */
#include "../utils/seq_realloc.h"      /* seq_t *seq_realloc(seq_t *p, idx_t n); */

seq_t a(idx_t n)
{
    /* Cache of found primes whose digit sum is 32 */
    static seq_t *primes = NULL;
    static size_t primes_cnt = 0;
    static size_t primes_cap = 0;
    static uint64_t next_candidate = 2;  /* Next integer to test */

    while (primes_cnt < (size_t)n) {
        /* Skip even numbers greater than 2 to speed up the search */
        if (next_candidate > 2 && (next_candidate & 1) == 0)
            next_candidate++;

        if (is_prime(next_candidate) &&
            digit_sum_uint64(next_candidate) == 32) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap == 0 ? 16 : primes_cap * 2;
                seq_t *tmp = seq_realloc(primes, new_cap);
                if (!tmp) abort();          /* allocation failure */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = (seq_t)next_candidate;
        }
        next_candidate++;
    }

    return primes[n - 1];
}
