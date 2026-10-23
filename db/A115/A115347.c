/*
 * A115347  Fibonacci primes that are also primes when their digits are
 *          reversed.
 * https://oeis.org/A115347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sequence A115347 – Fibonacci primes whose decimal reversal is
 *            also prime. Compute by iterating over Fibonacci numbers, testing
 *            each for primality and testing the reversal for primality
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime_big, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Cache of found terms (1‑based). */
    static seq_t *cache      = NULL;
    static size_t cache_used = 0;
    static size_t cache_cap  = 0;

    /* Current state of Fibonacci generation.  f_prev = F(k-1), f_curr = F(k). */
    static idx_t fib_index  = 1;   /* current Fibonacci index (starts at 1, F(1)=1) */
    static seq_t f_prev = 0;       /* F(0) */
    static seq_t f_curr = 1;       /* F(1) */

    /* Generate Fibonacci numbers until we have enough terms. */
    while (cache_used < (size_t)n) {
        /* Test the current Fibonacci number. */
        if (is_prime_big(f_curr)) {
            seq_t rev = reverse_decimal(f_curr);
            if (is_prime_big(rev)) {
                /* Store the qualifying Fibonacci number. */
                if (cache_used >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                    if (new_cap < cache_used + 1)
                        new_cap = cache_used + 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_used++] = f_curr;
            }
        }

        /* Advance to next Fibonacci number. */
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
        ++fib_index;
    }

    return cache[n - 1];
}
