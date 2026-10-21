/*
 * A092390  a(n) = prime(n) + prime(2n).
 * https://oeis.org/A092390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + prime(2n) (offset = 1)
 * Range:     u8 n <= 21, u16 n <= 2329, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Dynamically growable array of the first primes (1‑based indexing).  */
static uint64_t *prime_list     = NULL;
static size_t    prime_count    = 0;
static size_t    prime_capacity = 0;

/* Ensure that at least k primes are stored in prime_list (k ≥ 1). */
static void ensure_primes(idx_t k)
{
    if (prime_count >= (size_t)k)
        return;

    /* Grow storage as needed. */
    if (prime_capacity < (size_t)k) {
        size_t new_capacity = prime_capacity ? prime_capacity * 2 : 16;
        while (new_capacity < (size_t)k)
            new_capacity <<= 1;
        void *tmp = realloc(prime_list, new_capacity * sizeof(uint64_t));
        if (!tmp)
            exit(1);               /* out of memory */
        prime_list = (uint64_t *)tmp;
        prime_capacity = new_capacity;
    }

    /* Append primes. */
    if (prime_count == 0) {
        prime_list[prime_count++] = 2;   /* first prime */
        if (prime_count >= (size_t)k)
            return;
    }

    /* Next candidate after the last known prime. */
    uint64_t cand;
    if (prime_count == 1)
        cand = 3;                       /* skip the only known prime */
    else
        cand = prime_list[prime_count - 1] + 2;  /* start with the next odd number */

    while (prime_count < (size_t)k) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
            prime_list[prime_count++] = cand;

        cand += 2;          /* test next odd number */
    }
}

/* a(n) = prime(n) + prime(2n)   (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 1, but safe value */

    ensure_primes(2 * n);          /* primes up to the 2n-th one */
    seq_t p_n  = (seq_t)prime_list[n - 1];
    seq_t p_2n = (seq_t)prime_list[2 * n - 1];
    return p_n + p_2n;
}
