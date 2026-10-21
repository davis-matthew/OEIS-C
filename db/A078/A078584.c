/*
 * A078584  a(n) = prime(2n) - prime(2n-1).
 * https://oeis.org/A078584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(2*n) - prime(2*n-1) (OEIS A078584)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamically grow a list of primes.  prime_list[0] = 2. */
static uint32_t *prime_list = NULL;      /* array of primes (0‑based) */
static size_t    prime_count = 0;        /* number of primes stored so far */
static uint32_t  next_candidate = 3;     /* next odd number to test for primality */

/* Trial division test for a candidate. */
static bool is_prime(uint32_t p)
{
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t q = prime_list[i];
        if ((uint64_t)q * q > p)
            break;
        if (p % q == 0)
            return false;
    }
    return true;
}

/* Grow the prime list until it contains at least 'target' primes (1‑based). */
static void grow_primes_to(size_t target)
{
    if (prime_count >= target)
        return;                /* already enough primes */

    /* first prime */
    if (prime_count == 0) {
        prime_list = (uint32_t *)malloc(sizeof(uint32_t));
        if (!prime_list)
            return;          /* allocation failed; caller will handle 0 return */
        prime_list[0] = 2;
        prime_count = 1;
        next_candidate = 3;
    }

    /* extend until we have 'target' primes */
    while (prime_count < target) {
        uint32_t p = next_candidate;
        if (is_prime(p)) {
            uint32_t *new_list = (uint32_t *)realloc(prime_list,
                                                     (prime_count + 1) * sizeof(uint32_t));
            if (!new_list)
                return;      /* allocation failed; stop growing */
            prime_list = new_list;
            prime_list[prime_count++] = p;
        }
        next_candidate += 2;    /* only odd numbers need checking */
    }
}

/* The required sequence function.  idx_t is a large unsigned integer type. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;        /* undefined for n <= 0; return 0 */

    /* We need primes(2n) and primes(2n-1).  Ensure at least 2n primes are known. */
    size_t need = (size_t)(2 * n);
    grow_primes_to(need);

    /* If allocation failed or prime list still insufficient, return 0. */
    if (prime_count < need)
        return (seq_t)0;

    /* Compute the difference in seq_t. */
    seq_t diff = (seq_t)prime_list[need - 1] - (seq_t)prime_list[need - 2];
    return diff;
}
