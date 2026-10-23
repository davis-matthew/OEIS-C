/*
 * A107323  If n-th prime is 6m-1, then a(n) = 6m+1. If n-th prime is 6m+1,
 *          then a(n) = 6m-1.
 * https://oeis.org/A107323
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6m±1 where the prime p_n = 6m∓1 (m = (p_n±1)/6)
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* maintain a dynamic list of primes for repeated calls */
static uint64_t *prime_arr = NULL;
static size_t    prime_cap = 0;
static size_t    prime_cnt = 0;

static void ensure_prime_capacity(size_t min_cap)
{
    if (prime_cap < min_cap)
    {
        size_t new_cap = prime_cap ? prime_cap * 2 : 32;
        while (new_cap < min_cap) new_cap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(prime_arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();          /* fatal error if allocation fails */
        prime_arr = tmp;
        prime_cap = new_cap;
    }
}

/* fill prime_arr with the first n primes */
static void build_primes_until(size_t n)
{
    if (prime_cnt >= n) return;

    /* initialise first two primes */
    if (prime_cnt == 0)
    {
        ensure_prime_capacity(2);
        prime_arr[0] = 2;
        prime_arr[1] = 3;
        prime_cnt   = 2;
    }

    uint64_t candidate = prime_arr[prime_cnt-1] + 2;   /* next odd after last prime */
    while (prime_cnt < n)
    {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = prime_arr[i];
            if (p * p > candidate) break;       /* safe for our n limits */
            if (candidate % p == 0) { is_prime = false; break; }
        }

        if (is_prime)
        {
            ensure_prime_capacity(prime_cnt + 1);
            prime_arr[prime_cnt++] = candidate;
        }
        candidate += 2;
    }
}

/* --------------------------------------------------------------- */
/* Public sequence function                                         */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for offset <1 */

    /* generate primes up to the requested index */
    build_primes_until((size_t)n);

    uint64_t p   = prime_arr[n-1];        /* nth prime  (1‑based) */
    uint64_t res;

    /* flip the 6k±1 partner */
    if (p % 6 == 5)        res = p + 2;   /* p = 6m-1  → 6m+1 */
    else if (p % 6 == 1)   res = p - 2;   /* p = 6m+1  → 6m-1 */
    else                   res = 0;        /* impossible for primes >3 */

    return (seq_t)res;
}
