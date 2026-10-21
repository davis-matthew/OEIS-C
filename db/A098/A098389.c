/*
 * A098389  Prime(n) - floor(log_2(prime(n))).
 * https://oeis.org/A098389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - floor(log_2(prime(n)))
 * Range:     u8 n <= 56, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper for floor(log2(x)) for 64‑bit unsigned integers. */
static inline uint32_t floor_log2_u64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll(x);
#else
    uint32_t r = 0;
    while (x >>= 1) ++r;
    return r;
#endif
}

/* Dynamically maintain the list of primes. */
static uint64_t *prime_list = NULL;      /* array of primes */
static size_t prime_count = 0;           /* number of primes stored */
static size_t prime_capacity = 0;        /* allocated capacity of the array */
static uint64_t next_candidate = 3;      /* next odd number to test for primality */

/* Ensure the list contains at least n primes (n >= 1). */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    /* Initial prime 2 if list is empty. */
    if (prime_count == 0) {
        prime_capacity = 16;
        prime_list = (uint64_t *)malloc(prime_capacity * sizeof(uint64_t));
        prime_list[0] = 2;
        prime_count = 1;
        next_candidate = 3;
    }

    /* Grow the array when necessary. */
    if (prime_capacity < prime_count + 16) {
        size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1); /* allocation failed */
        prime_list = tmp;
        prime_capacity = new_cap;
    }

    /* Sieve by trial division using the primes already found. */
    while (prime_count < (size_t)n) {
        uint64_t p = next_candidate;
        next_candidate += 2;                /* only odd candidates */

        bool is_prime = true;
        for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= p; ++i) {
            if (p % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count >= prime_capacity) {
                size_t new_cap = prime_capacity * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                prime_list = tmp;
                prime_capacity = new_cap;
            }
            prime_list[prime_count++] = p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard for safety */

    ensure_primes(n);

    uint64_t p = prime_list[n - 1];          /* nth prime */
    uint32_t lg = floor_log2_u64(p);         /* floor(log2(p)) */
    return (seq_t)p - (seq_t)lg;
}
