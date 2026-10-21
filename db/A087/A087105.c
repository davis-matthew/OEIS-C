/*
 * A087105  (prime(n-1) + 1)*(prime(n+1) - 1).
 * https://oeis.org/A087105
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n-1)+1)*(prime(n+1)-1)
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 6543, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"

/* Static storage for primes discovered so far.  prime[0] = 2, prime[1] = 3, ... */
static uint64_t *prime_list = NULL;
static size_t prime_cap   = 0;
static size_t prime_cnt   = 0;   /* number of primes stored */
static uint64_t next_candidate = 3;

/* Return the k-th prime (k >= 1), using memoised list. */
static uint64_t kth_prime(idx_t k)
{
    if (k == 0) return 0;                /* undefined, but never called with 0 */

    /* Ensure list has at least k primes. */
    while (prime_cnt < (size_t)k)
    {
        /* Grow array if needed. */
        if (prime_cnt >= prime_cap)
        {
            size_t new_cap = prime_cap ? prime_cap * 2 : 8;
            uint64_t *new_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
            if (!new_list) exit(EXIT_FAILURE);   /* out of memory */
            prime_list = new_list;
            prime_cap  = new_cap;
        }

        /* Find the next prime.  Skip even numbers. */
        while (!is_prime(next_candidate))
            next_candidate += 2;

        /* Store the found prime. */
        if (prime_cnt == 0)
            prime_list[prime_cnt++] = 2;   /* first prime is 2, handled separately */
        prime_list[prime_cnt++] = next_candidate;

        /* Advance to the next odd candidate. */
        next_candidate += 2;
    }

    /* prime_list is 0‑based but primes are numbered starting at 1. */
    return prime_list[k - 1];
}

/* OEIS sequence A087105.  Offsets: a(2) is first term. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* value for undefined indices is irrelevant */

    seq_t p_left  = (seq_t)kth_prime(n - 1);
    seq_t p_right = (seq_t)kth_prime(n + 1);

    return (p_left + 1) * (p_right - 1);
}
