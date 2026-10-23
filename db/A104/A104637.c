/*
 * A104637  Number of even digits in n-th prime.
 * https://oeis.org/A104637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = number of even decimal digits in the n‑th prime
 * Range:     u8 n <= 54, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   count_even_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple trial‑division prime generator. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/count_even_digits.h"

/* ------------------------------------------------------------------ */
/* Global storage for the list of primes.  The size is capped at
 * prime_cap and contains prime_count primes.  current_limit is the
 * largest integer that has been tested for primality. */
static seq_t *prime_list = NULL;
static idx_t prime_count = 0;
static idx_t prime_cap   = 0;
static idx_t current_limit = 1;

/* ------------------------------------------------------------------ */
/* Ensure that prime_list contains at least the first n primes. */
static void ensure_primes(idx_t n)
{
    if (n <= prime_count)
        return;                       /* already have enough */

    /* Rough upper bound for the n-th prime.  For n ≤ 10 000 this
     * linear estimate is more than enough. */
    idx_t needed = (n < 6 ? 15 : n * 20);
    if (needed <= current_limit)
        needed = current_limit + 1;

    /* Grow the capacity of prime_list as required. */
    idx_t required_cap = prime_count + (needed - current_limit);
    while (prime_cap < required_cap) {
        size_t new_cap = (prime_cap == 0) ? 1024 : (size_t)prime_cap * 2;
        seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        prime_list = tmp;
        prime_cap  = (idx_t)new_cap;
    }

    /* Trial division from current_limit+1 up to needed. */
    for (idx_t cand = current_limit + 1; cand <= needed; ++cand) {
        idx_t r = (idx_t)sqrt((double)cand);    /* small enough for double */
        int is_prime = 1;
        for (idx_t i = 0; i < prime_count && prime_list[i] <= r; ++i) {
            if (cand % prime_list[i] == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (prime_count >= prime_cap) {      /* defensive check */
                size_t new_cap = prime_cap * 2;
                seq_t *tmp = (seq_t *)realloc(prime_list,
                                              new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
                prime_cap  = (idx_t)new_cap;
            }
            prime_list[prime_count++] = cand;
        }
    }
    current_limit = needed;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Main sequence function.  External linkage is default. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n = 0 */

    ensure_primes(n);
    seq_t prime = prime_list[n - 1];   /* primes are 0‑based in the array */
    return count_even_digits(prime);
}
