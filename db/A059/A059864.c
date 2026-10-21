/*
 * A059864  a(n) = Product_{i=4..n} (prime(i) - 5).
 * https://oeis.org/A059864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=4..n} (prime(i) - 5) with offset 1
 *            (a(1)=a(2)=a(3)=1). The product is taken over the primes in
 *            0‑based indexing: prime(4)=7, prime(5)=11, etc
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

 
/* --- cache for computed sequence values (1‑based) --- */
static seq_t *seq_cache = NULL;
static size_t seq_cap  = 0;

/* --- storage for accumulated primes (0‑based: prime_list[0] = 2) --- */
static uint64_t *prime_list = NULL;
static size_t   prime_cap  = 0;
static size_t   prime_cnt  = 0;

/* Ensure prime_list holds at least n primes. */
static void ensure_primes(idx_t n) {
    if (prime_cnt >= (size_t)n)
        return;

    /* Grow storage as needed. */
    if (prime_cap < (size_t)n) {
        if (prime_cap == 0)
            prime_cap = 16;
        while (prime_cap < (size_t)n)
            prime_cap <<= 1;
        prime_list = (uint64_t *)realloc(prime_list,
                                         prime_cap * sizeof(*prime_list));
    }

    /* Scan for new primes starting from the next candidate. */
    uint64_t cand = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;
    if (cand % 2 == 0 && cand != 2)
        ++cand;          /* skip even numbers after 2 */

    while (prime_cnt < (size_t)n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_cnt++] = cand;
        }
        cand += (cand == 2) ? 1 : 2;   /* after 2, test only odds */
    }
}

/* Ensure seq_cache can hold values up to n and fill missing terms. */
static void ensure_seq(idx_t n) {
    if (n == 0)
        return;

    /* Grow cache if necessary. */
    if (seq_cap < (size_t)n + 1) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;
        seq_cache = (seq_t *)realloc(seq_cache,
                                     new_cap * sizeof(*seq_cache));
        seq_cap = new_cap;
    }

    /* Compute sequence values up to n from scratch. */
    seq_cache[1] = 1;
    if (n >= 2)
        seq_cache[2] = 1;
    if (n >= 3)
        seq_cache[3] = 1;
    if (n >= 4) {
        seq_cache[4] = (seq_t)(prime_list[3] - 5);   /* prime(4)=7 → 2 */
        for (size_t i = 5; i <= (size_t)n; ++i) {
            seq_t factor = (seq_t)(prime_list[i - 1] - 5); /* prime(i) */
            seq_cache[i] = seq_cache[i - 1] * factor;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                    /* undefined for offset 0 */

    ensure_primes(n);                /* need at least n primes */
    ensure_seq(n);                   /* compute values up to n */

    return seq_cache[n];
}
