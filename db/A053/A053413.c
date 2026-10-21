/*
 * A053413  Primes of the form A003266(n) + 1.
 * https://oeis.org/A053413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth prime of the form A003266(k)+1, where
 * A003266(k) = product of the first k Fibonacci numbers.  The terms
 * are listed in OEIS A053413 with offset 1. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static state to allow out‑of‑order calls and memoisation. */
    static seq_t product  = 1;     /* Product of the first k Fibonacci numbers. */
    static seq_t fib_prev = 0;     /* F(k-1) */
    static seq_t fib_curr = 1;     /* F(k)   */
    static seq_t *primes  = NULL;  /* Array of found primes. */
    static size_t primes_cnt = 0;  /* Number of primes stored. */
    static size_t primes_cap = 0;  /* Current capacity of the primes array. */

    if (n == 0) return 0;          /* Offset is 1 – behaviour for n<1 is irrelevant. */

    /* Generate primes until we have at least n of them. */
    while (primes_cnt < n) {
        /* Multiply the running product by the current Fibonacci number */
        product *= fib_curr;               /* For large builds this uses arbitrary‑precision. */

        /* Candidate prime */
        seq_t candidate = product + 1;

        /* Test primality and store if prime. */
        if (is_prime_big(candidate)) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? (primes_cap << 1) : 4;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failure – abort safely. */
                    exit(EXIT_FAILURE);
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = candidate;
        }

        /* Advance to the next Fibonacci number. */
        seq_t next_fib = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next_fib;
    }

    return primes[n - 1];
}
