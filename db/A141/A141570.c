/*
 * A141570  Primes of the form 2*3*5*7*n+83.
 * https://oeis.org/A141570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime of the form 210·k + 83, with k ≥ 0
 * Range:     u8 n <= 1, u16 n <= 134, u32 n <= 146, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* External helper functions */
extern bool is_prime_big(seq_t n);
extern void grow_seq(seq_t **arr, size_t *cap, size_t need);

/* Return the n-th term of OEIS A141570.
 * n must be ≥ 1. For n < 1 the function returns 0. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Cached primes computed so far and state of enumeration. */
    static seq_t *cache   = NULL;   /* array of primes 210*k+83 found */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of cached primes */
    static idx_t   next_k   = 0;     /* next integer k to test */

    /* Generate primes until we have at least n of them. */
    while (cache_len < n) {
        /* 210 = 2·3·5·7 */
        seq_t candidate = (seq_t)210 * (seq_t)next_k + (seq_t)83;

        if (is_prime_big(candidate)) {
            /* Ensure there is space for the new prime. */
            if (cache_len + 1 > cache_cap) {
                /* grow_seq will increase the capacity to at least `need`. */
                grow_seq(&cache, &cache_cap, cache_len + 1);
            }
            cache[cache_len++] = candidate;
        }
        next_k++;  /* proceed to the next k regardless of primality */
    }

    /* cache[n‑1] holds the nth term (1‑based index). */
    return cache[n - 1];
}
