/*
 * A061466  Product of primes prime(3*n+1), prime(3*n+2), prime(3*n+3).
 * https://oeis.org/A061466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = p(3n+1) * p(3n+2) * p(3n+3) (prime indices are 1‑based)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 85, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Global storage for the list of primes */
static uint64_t *prime_list = NULL;    /* array of primes (1‑based for simplicity) */
static size_t  prime_count = 0;        /* number of primes stored */
static size_t  sieve_limit = 0;        /* upper bound that has been sieved */

/* Perform a sieve up to 'limit' (inclusive) and rebuild prime_list. */
static void do_sieve(size_t limit)
{
    /* Allocate a simple boolean array for the sieve */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return;        /* allocation failure – unlikely in test harness */

    /* Initialise all entries as prime */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    /* Classic Eratosthenes sieve */
    size_t sqrt_limit = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= sqrt_limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    /* Count primes to know array size */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++count;

    /* Allocate a new prime array (1‑based index, primes[1] = 2) */
    uint64_t *new_list = (uint64_t *)malloc((count + 1) * sizeof(uint64_t));
    if (!new_list) {
        free(is_prime);
        return;              /* allocation failure – unlikely */
    }

    /* Store primes */
    size_t idx = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            new_list[idx++] = (uint64_t)i;
        }
    }

    /* Replace the old list */
    free(prime_list);
    prime_list = new_list;
    prime_count = count;
    sieve_limit  = limit;

    free(is_prime);
}

/* Ensure that prime_list contains at least (need_index+1) primes (0‑based). */
static void ensure_primes_needed(size_t need_index)
{
    /* need_index is 0‑based; we need prime_count > need_index */
    if (prime_count > need_index) return;

    size_t new_limit = sieve_limit ? sieve_limit * 2 : 100;
    while (prime_count <= need_index) {
        do_sieve(new_limit);
        new_limit *= 2;       /* increase limit until we have enough */
    }
}

/* Main OEIS function */
seq_t a(idx_t n)
{
    /* We need primes at 1‑based positions 3n+1, 3n+2, 3n+3.
       Convert to 0‑based indices for our array (prime_list[1] = 2). */
    size_t need_index = 3 * (size_t)n + 2;   /* 0‑based last needed index */

    ensure_primes_needed(need_index);

    seq_t p1 = (seq_t)prime_list[3 * (size_t)n + 1];
    seq_t p2 = (seq_t)prime_list[3 * (size_t)n + 2];
    seq_t p3 = (seq_t)prime_list[3 * (size_t)n + 3];

    return p1 * p2 * p3;
}
