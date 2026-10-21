/*
 * A073123  a(n) is the largest number such that pi(a(n)) = prime(n).
 * https://oeis.org/A073123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(A000040(n)+1) - 1
 * Range:     u8 n <= 16, u16 n <= 844, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   estimate_prime_value_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the largest m with π(m) = p_n, where p_n is the n‑th prime. */

#include <stdlib.h>
#include <math.h>
#include "../utils/estimate_prime_value_limit.h"

/* static storage for the prime table */
static uint64_t *prime_table = NULL;   /* array of primes (ascending) */
static size_t  prime_count  = 0;       /* number of primes stored */
static idx_t   prime_limit_val = 0;    /* last value limit used for sieving */

/* generate all primes <= limit using a simple sieve of Eratosthenes.
   The result is stored in prime_table; prime_count holds the number of primes. */
static void generate_primes_upto(idx_t limit) {
    /* free previous list */
    if (prime_table) free(prime_table);
    prime_table = NULL;
    prime_count = 0;
    prime_limit_val = limit;

    /* sieve array: 0 = prime, 1 = composite */
    char *is_comp = (char *)calloc((size_t)(limit + 1), sizeof(char));
    if (!is_comp) return;   /* allocation failed */

    /* dynamic array for primes */
    size_t capacity = 1024;
    prime_table = (uint64_t *)malloc(capacity * sizeof(uint64_t));
    if (!prime_table) {
        free(is_comp);
        return;
    }

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            /* store the prime */
            if (prime_count >= capacity) {
                capacity *= 2;
                prime_table = (uint64_t *)realloc(prime_table, capacity * sizeof(uint64_t));
                if (!prime_table) break;   /* allocation failed */
            }
            prime_table[prime_count++] = i;

            /* mark composites */
            if ((int64_t)i * i <= limit)
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
        }
    }
    free(is_comp);
}

/* ensure at least 'need_index' primes are available in prime_table. */
static void ensure_primes_needed(idx_t need_index) {
    if (prime_count >= need_index) return;   /* already have enough */

    idx_t needed_limit = estimate_prime_value_limit(need_index);
    if (prime_limit_val >= needed_limit) return;  /* current sieve covers it */

    generate_primes_upto(needed_limit);
}

/* --------------------------------------------------------------- */
/*  Main sequence function
/* --------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for offset 1, but guard anyway */

    /* prime p_n */
    ensure_primes_needed(n);
    seq_t p_n = (seq_t)prime_table[n - 1];

    /* index of the next prime after p_n */
    idx_t next_index = (idx_t)p_n + 1;
    ensure_primes_needed(next_index);
    seq_t p_next = (seq_t)prime_table[next_index - 1];

    return p_next - 1;
}
