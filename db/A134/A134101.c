/*
 * A134101  Odd nonprimes such that the prior odd number is not a prime but
 *          the next odd number is a prime.
 * https://oeis.org/A134101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate odd numbers, keeping those that satisfy
 * (odd non‑prime, previous odd non‑prime, next odd prime).  The
 * function memoises found terms, so calls may arrive in any order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Note: seq_t and idx_t are provided by the build environment.
 * They may be an unsigned integer type or an arbitrary‑precision
 * integer class.  This implementation works with either. */

seq_t a(idx_t n)
{
    /* Memoised cache for previously found terms. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* The next odd integer to examine (starting from 3). */
    static idx_t next_odd = 3;

    if (n < 1) {
        /* Undefined for n < 1; return 0 safely. */
        return (seq_t)0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Convert candidate odd numbers to seq_t for primality tests. */
        seq_t odd_seq  = (seq_t)next_odd;
        seq_t prev_seq = (seq_t)(next_odd - 2);
        seq_t next_seq = (seq_t)(next_odd + 2);

        /* Check the sequence condition. */
        if (!is_prime_big(odd_seq) &&
            !is_prime_big(prev_seq) &&
            is_prime_big(next_seq)) {

            /* Grow cache if it is full. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Memory allocation failed; abort computation. */
                    return (seq_t)0;
                }
                cache     = tmp;
                cache_cap = new_cap;
            }

            cache[cache_len++] = odd_seq;

            /* If we have reached the requested element, stop scanning. */
            if (cache_len == (size_t)n) {
                /* Advance past the current odd for future calls. */
                next_odd += 2;
                break;
            }
        }

        /* Move to the next odd number. */
        next_odd += 2;
    }

    return cache[n - 1];
}
