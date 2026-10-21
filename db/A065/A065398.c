/*
 * A065398  Fibonacci numbers whose digits sum to a prime.
 * https://oeis.org/A065398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 35, u128 n <= 53,
 *            big any n
 * Helpers:   digit_sum, ensure_capacity, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A065398: Fibonacci numbers whose digit sum is a prime. */
/* The sequence a(n) is obtained by selecting Fibonacci numbers F_k with
   digit_sum(F_k) prime, and returning them in increasing order of k. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/digit_sum.h"     /* seq_t digit_sum(seq_t); */
#include "../utils/is_prime_small.h" /* bool is_prime_small(seq_t); */
#include "../utils/ensure_capacity.h" /* void ensure_capacity(size_t needed,
                                          size_t *cap, void **ptr, size_t elem_size); */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;        /* array of found terms      */
    static size_t cache_size = 0;      /* number of terms cached   */
    static size_t cache_cap  = 0;      /* allocated capacity       */

    /* keep track of Fibonacci recurrence across calls */
    static seq_t fib_prev = 1, fib_curr = 1; /* fib(k-1), fib(k)  */
    static bool init = false;                 /* initialisation flag */

    /* single initialisation */
    if (!init) {
        init = true;
        /* nothing else to do, state already set to F1=1, F2=1   */
    }

    /* if the desired term is already cached, return it immediately */
    if (n > 0 && (size_t)(n - 1) < cache_size)
        return cache[(size_t)(n - 1)];

    /* generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        /* compute next Fibonacci number */
        seq_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;       /* now fib_prev=F_{k-1}, fib_curr=F_k */

        /* test if digit sum of this Fibonacci number is prime */
        seq_t sum = digit_sum(fib_next);
        if (is_prime_small(sum)) {
            /* grow cache if necessary */
            ensure_capacity(cache_size + 1, &cache_cap, (void **)&cache,
                            sizeof(seq_t));
            cache[cache_size++] = fib_next;
        }
    }

    /* return the requested term */
    return cache[(size_t)(n - 1)];
}
