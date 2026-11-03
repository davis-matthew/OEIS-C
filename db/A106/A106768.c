/*
 * A106768  Primes with digit sum = 32.
 * https://oeis.org/A106768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 131, u32 n <= 6200, u64 n <= 6200,
 *            u128 n <= 6200, big any n
 * Validated: standard, pass, 50 terms through n = 5949 (2026-11-03)
 * Helpers:   digit_sum_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes with digit sum 32.  Computed by search. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_sum_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

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

        if (is_prime_u64(next_candidate) &&
            digit_sum_u64(next_candidate) == 32) {
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
