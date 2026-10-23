/*
 * A139686  Odd multiplicative orders of 2 modulo primes.
 * https://oeis.org/A139686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = multiplicative order of 2 modulo the n‑th odd prime. The
 *            function memoizes orders so that subsequent calls in any order
 *            are efficient
 * Range:     u8 n <= 17, u16 n <= 2471, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/mod_pow.h"
#include "../utils/next_prime.h"

/* Return the n-th term of OEIS A139686. */
seq_t a(idx_t n)
{
    static seq_t *orders = NULL;   /* cached orders */
    static size_t orders_len = 0;  /* number of stored orders */
    static size_t orders_cap = 0;  /* current capacity of orders array */

    static uint64_t last_prime = 2;   /* start before first odd prime */

    if (n == 0)
        return (seq_t)0;  /* values before offset are undefined */

    /* Grow cache until we have at least n values. */
    while (orders_len < n)
    {
        /* get next prime greater than last_prime (skipping 2 automatically) */
        last_prime = next_prime(last_prime);

        /* compute multiplicative order of 2 mod last_prime */
        uint64_t p = last_prime;          /* clarify type */
        seq_t   ord = (seq_t)(p - 1);      /* start with p-1 */

        /* factor p-1 by trial division to obtain distinct prime factors */
        uint64_t tmp = p - 1;
        for (uint64_t f = 2; f * f <= tmp; ++f)
        {
            if (tmp % f == 0)
            {
                /* remove all copies of f from tmp to get distinct factor */
                while (tmp % f == 0) tmp /= f;

                /* reduce order while possible */
                while (ord % (seq_t)f == 0 &&
                       mod_pow((seq_t)2, (uint64_t)(ord / (seq_t)f), (seq_t)p) == (seq_t)1)
                {
                    ord /= (seq_t)f;
                }
            }
        }

        /* remaining factor > 1, if any */
        if (tmp > 1)
        {
            uint64_t f = tmp;
            while (ord % (seq_t)f == 0 &&
                   mod_pow((seq_t)2, (uint64_t)(ord / (seq_t)f), (seq_t)p) == (seq_t)1)
            {
                ord /= (seq_t)f;
            }
        }

        /* store only odd orders */
        if ((ord & (seq_t)1) != 0)
        {
            if (orders_len == orders_cap)
            {
                size_t new_cap = orders_cap ? orders_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(orders, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);  /* out of memory */
                orders = tmp;
                orders_cap = new_cap;
            }
            orders[orders_len++] = ord;
        }
    }

    return orders[n - 1];
}
