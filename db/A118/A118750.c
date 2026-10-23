/*
 * A118750  a(n) = product[k=1..n] P(k), where P(k) is the largest prime <=
 *          3*k. a(n) = product[k=1..n] A118749(k).
 * https://oeis.org/A118750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = product_{k=1..n} P(k) where P(k) is the largest prime ≤
 *            3*k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The function relies only on the standard library.
   It keeps static caches for primes and partial products
   so that calls can be made in any order. */

seq_t a(idx_t n)
{
    /* ----- Static caches ------------------------------------------ */
    /* last n for which cached_prod has been filled */
    static idx_t cached_max_n = 0;
    /* array of products: cached_prod[k] = a(k) for 1 ≤ k ≤ cached_max_n */
    static seq_t *cached_prod = NULL;
    static size_t prod_cap   = 0;

    /* prime tables up to last needed limit */
    static idx_t prime_limit = 0;          /* 3 * largest n filled */
    static uint16_t *largest_prime_le = NULL;   /* 1‑based index */
    static uint8_t *sieve_buf  = NULL;   /* boolean buffer for sieve */
    static size_t sieve_cap   = 0;

    /* ----- Guard for out‑of‑range indices ------------------------- */
    if (n == 0) {
        /* The sequence is defined for n ≥ 1, but return 0 for safety. */
        return (seq_t)0;
    }

    /* ----- Ensure prime data up to 3*n ---------------------------- */
    idx_t needed_limit = 3 * n;
    if (needed_limit > prime_limit) {

        /* Allocate fresh buffers for the new size. */
        if (sieve_buf) {
            free((void*)sieve_buf);
        }
        sieve_buf = (uint8_t*)malloc((needed_limit + 1) * sizeof(uint8_t));
        if (!sieve_buf) exit(EXIT_FAILURE);
        sieve_cap = needed_limit + 1;

        /* Simple Eratosthenes sieve */
        for (idx_t i = 0; i <= needed_limit; ++i) {
            sieve_buf[i] = 1;
        }
        if (needed_limit >= 0) sieve_buf[0] = 0;
        if (needed_limit >= 1) sieve_buf[1] = 0;

        for (idx_t p = 2; p * p <= needed_limit; ++p) {
            if (sieve_buf[p]) {
                for (idx_t j = p * p; j <= needed_limit; j += p) {
                    sieve_buf[j] = 0;
                }
            }
        }

        /* Build array of largest prime ≤ i. */
        if (largest_prime_le) {
            free((void*)largest_prime_le);
        }
        largest_prime_le = (uint16_t*)malloc((needed_limit + 1) * sizeof(uint16_t));
        if (!largest_prime_le) exit(EXIT_FAILURE);

        uint16_t last_prime = 0;
        for (idx_t i = 1; i <= needed_limit; ++i) {
            if (sieve_buf[i]) {
                last_prime = (uint16_t)i;
            }
            largest_prime_le[i] = last_prime;
        }

        prime_limit = needed_limit;
    }

    /* ----- Compute product values up to n if needed -------------- */
    if (n > cached_max_n) {
        /* Ensure enough capacity for cached_prod. */
        size_t new_cap = (size_t)(n + 1);   /* indices 0..n, 0 unused */
        if (new_cap > prod_cap) {
            seq_t *new_arr = (seq_t*)realloc((void*)cached_prod, new_cap * sizeof(seq_t));
            if (!new_arr) exit(EXIT_FAILURE);
            cached_prod = new_arr;
            prod_cap    = new_cap;
        }

        /* Start from the last known product. */
        seq_t prod = 1;
        if (cached_max_n > 0) {
            prod = cached_prod[(size_t)cached_max_n];
        }

        /* Extend the product array. */
        for (idx_t k = cached_max_n + 1; k <= n; ++k) {
            seq_t pk = (seq_t)largest_prime_le[3 * k];
            prod = prod * pk;
            cached_prod[(size_t)k] = prod;
        }
        cached_max_n = n;
    }

    /* ----- Return the requested term ----------------------------- */
    return cached_prod[(size_t)n];
}
