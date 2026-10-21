/*
 * A086360  The n-th primorial number reduced modulo 9.
 * https://oeis.org/A086360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = primorial(n) mod 9
 * Range:     u8 n <= 9891, u16 n <= 9891, u32 n <= 9891, u64 n <= 9891,
 *            u128 n <= 9891, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---- static cache ---------------------------------------------------- */
    static bool initialized = false;
    static seq_t prod_mod9 = 1;            /* product of processed primes mod 9 */
    static idx_t primes_size = 0;
    static idx_t primes_cap = 0;
    static uint64_t *primes = NULL;       /* list of found primes */
    static idx_t values_size = 0;
    static idx_t values_cap = 0;
    static seq_t *values = NULL;          /* a(0) ... a(values_size-1) */

    if (!initialized) {
        /* initial allocation */
        values_cap = 8;
        values = (seq_t*)malloc(values_cap * sizeof(seq_t));
        if (!values) abort();
        values[0] = (seq_t)1;  /* a(0) = 1 (empty product) */
        values_size = 1;

        primes_cap = 8;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) abort();
        primes_size = 0;

        initialized = true;
    }

    /* ---- extend cache up to requested n -------------------------------- */
    while (values_size <= n) {
        /* find next prime */
        uint64_t candidate = (primes_size == 0) ? 2ULL : primes[primes_size - 1] + 1ULL;

        /* loop until a prime is found */
        for (;;) {
            bool prime = true;
            for (idx_t i = 0; i < primes_size; ++i) {
                uint64_t p = primes[i];
                if (p > candidate / p) break;       /* p*p > candidate */
                if (candidate % p == 0) {
                    prime = false;
                    break;
                }
            }
            if (prime) break;   /* candidate is prime */
            ++candidate;
        }

        /* append new prime to the list */
        if (primes_size == primes_cap) {
            idx_t new_cap = (primes_cap ? primes_cap * 2 : 8);
            uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_size++] = candidate;

        /* update product modulo 9 */
        seq_t mult = (seq_t)(candidate % 9ULL);
        prod_mod9 = (seq_t)((prod_mod9 * mult) % 9);

        /* store the new sequence value */
        if (values_size == values_cap) {
            idx_t new_cap = (values_cap ? values_cap * 2 : 8);
            seq_t *tmp = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            values = tmp;
            values_cap = new_cap;
        }
        values[values_size++] = prod_mod9;
    }

    return values[n];
}
