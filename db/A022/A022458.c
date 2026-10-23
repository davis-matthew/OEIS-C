/*
 * A022458  a(n) = prime(2n-1) mod prime(n).
 * https://oeis.org/A022458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(2n-1) mod prime(n)
 * Range:     u8 n <= 166, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, NULL */
#include <stdint.h>   /* for uint32_t */
#include <stdlib.h>   /* for malloc, realloc */
#include <stdbool.h>  /* for bool */

/* Statically allocated list of primes */
static uint32_t *prime_list = NULL;   /* array of primes */
static size_t   prime_count = 0;      /* number of primes currently stored */
static size_t   prime_cap   = 0;      /* allocated capacity of prime_list */

/* Grow the prime list so that it contains at least 'need' primes. */
static void ensure_prime_index(size_t need)
{
    if (need <= prime_count)
        return;                       /* already have enough primes */

    /* Allocate more space if necessary. */
    if (need > prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        while (new_cap < need)
            new_cap *= 2;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) abort();                /* out of memory */
        prime_list = tmp;
        prime_cap = new_cap;
    }

    /* Start searching from the next integer after the last stored prime. */
    uint32_t cand = prime_count ? prime_list[prime_count - 1] + 1 : 2;

    /* Generate primes until we have 'need' of them. */
    for (; prime_count < need; ++cand) {
        bool is_prime = true;
        /* Trial division by primes already found. */
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            uint64_t pp = (uint64_t)p * p;
            if (pp > cand)                /* p > sqrt(cand) -> stop */
                break;
            if (cand % p == 0) {          /* composite */
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_count++] = cand;
        }
    }
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the offset 1, but return 0 safely */
        return (seq_t)0;

    /* We need the nth prime and the (2n-1)th prime. */
    size_t need = (size_t)((idx_t)2 * n - 1);
    ensure_prime_index(need);

    uint32_t p_n  = prime_list[(size_t)(n - 1)];
    uint32_t p_2n = prime_list[need - 1];

    return (seq_t)(p_2n % p_n);
}
