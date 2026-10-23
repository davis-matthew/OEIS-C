/*
 * A031390  a(n) = prime(9*n - 3).
 * https://oeis.org/A031390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(9*n - 3)
 * Range:     u8 n <= 6, u16 n <= 727, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cache of primes */
static seq_t *prime_cache = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static uint64_t next_candidate = 2;

/* Ensure the prime cache has space for at least 'needed' primes */
static void grow_cache(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    prime_cache = tmp;
    prime_cap = new_cap;
}

/* Find the next prime and store it in the cache */
static void generate_next_prime(void)
{
    /* First time: initialise with 2 */
    if (prime_cache == NULL) {
        grow_cache(64);
        prime_cache[0] = (seq_t)2;
        prime_cnt = 1;
        next_candidate = 3;          /* start with the next odd number */
        return;
    }

    /* Find the next prime starting from next_candidate (odd only) */
    while (true) {
        uint64_t cand = next_candidate;
        bool is_prime = true;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = (uint64_t)prime_cache[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            grow_cache(prime_cnt + 1);
            prime_cache[prime_cnt++] = (seq_t)cand;
            next_candidate += 2;
            break;
        }
        next_candidate += 2;
    }
}

/* Ensure the cache contains at least 'target' primes (1‑based index) */
static void ensure_primes(idx_t target)
{
    while (prime_cnt < target)
        generate_next_prime();
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    idx_t needed = (idx_t)(9 * n - 3);   /* index of the desired prime */
    if (needed == 0)                     /* safety for n < 1, though not used */
        return (seq_t)0;
    ensure_primes(needed);
    return prime_cache[needed - 1];
}
