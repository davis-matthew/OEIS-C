/*
 * A104073  Primes of the form 2^n+7^2.
 * https://oeis.org/A104073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime, isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes of the form 2^n + 7^2.  The function searches for n
 * such that 2^n + 49 is prime and returns the nth such prime.  It uses
 * a simple cache to avoid recomputing primes on subsequent calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* bool is_prime(uint64_t n); */
#include "../utils/isprime_big.h"   /* bool isprime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* Static cache of computed primes and next exponent to test. */
    static seq_t *primes   = NULL;
    static idx_t   size    = 0;
    static idx_t   capacity = 0;
    static idx_t   next_exp = 1;          /* next exponent to test (n = 1). */

    /* Initialise the cache on first call. */
    if (primes == NULL) {
        capacity = 16;
        primes   = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* Extend the cache until we have at least n primes. */
    while (size < n) {
        seq_t val = ((seq_t)1 << next_exp) + (seq_t)49;
        bool ok;

        /* Pick a primality test suitable for the size of seq_t. */
        if (sizeof(seq_t) <= 8) {          /* fits into uint64_t */
            ok = is_prime((uint64_t)val);
        } else {                            /* arbitrary‑precision */
            ok = isprime_big(val);
        }

        if (ok) {                          /* store the prime */
            if (size == capacity) {
                capacity *= 2;
                primes = (seq_t *)realloc(primes,
                                          capacity * sizeof(seq_t));
            }
            primes[size++] = val;
        }
        next_exp++;                         /* move to the next exponent */
    }

    /* Return the requested nth prime (1‑based index). */
    return primes[n - 1];
}
