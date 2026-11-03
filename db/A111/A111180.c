/*
 * A111180  Product_{k=1..n} prime(k)!.
 * https://oeis.org/A111180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{k=1..n} prime(k)!, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 8 (2026-11-03)
 * Helpers:   is_prime_trial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_trial.h"
#include "../utils/mem.h"

/*--- dynamic array helpers for primes --------------------------------------*/
static void ensure_primes_cap(size_t *cap, uint64_t **arr, size_t min_cap) {
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t get_prime(uint64_t idx) {
    static uint64_t *prime_vec = NULL;
    static size_t prime_cap = 0;
    static size_t prime_sz  = 0;
    static uint64_t candidate = 3;      /* next odd to test */

    if (!prime_vec) { /* first call: initialise */
        ensure_primes_cap(&prime_cap, &prime_vec, 16);
        prime_vec[0] = 2;
        prime_sz = 1;
    }

    while (prime_sz < idx) {
        /* find next prime */
        while (!is_prime_trial(candidate, prime_vec, prime_sz))
            candidate += 2;

        /* add found prime */
        if (prime_sz == prime_cap)
            ensure_primes_cap(&prime_cap, &prime_vec, prime_cap << 1);

        prime_vec[prime_sz++] = candidate;
        candidate += 2;
    }
    return (seq_t)prime_vec[idx - 1];
}

seq_t a(idx_t n)
{
    /* Memoization state */
    static seq_t *memo          = NULL;    /* array of a(k) values */
    static size_t cap           = 0;       /* current allocation size */
    static idx_t last_n         = 0;       /* largest n computed */
    static seq_t product        = 1;       /* product of prime(k)! up to last_n */
    static seq_t fact           = 1;       /* factorial of current prime */
    static seq_t prev_prime     = 1;       /* previous prime processed (starts at 1) */

    /* Allocate memo array on first call */
    if (memo == NULL) {
        /* Ensure at least one element for a(0) */
        cap = 1;
        memo = mem_new(seq_t, cap);
        if (memo == NULL) exit(EXIT_FAILURE);
        memo[0] = 1;          /* a(0) = 1 */
        last_n = 0;
    }

    /* Grow memo array if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap *= 2;
        seq_t *tmp = mem_resize(seq_t, memo, newcap);
        if (tmp == NULL) exit(EXIT_FAILURE);
        memo = tmp;
        cap  = newcap;
    }

    /* Return cached value if already computed */
    if (n <= last_n)
        return memo[(size_t)n];

    /* Compute terms up to n */
    for (idx_t k = last_n + 1; k <= n; ++k) {
        /* Get k‑th prime (1‑indexed) */
        seq_t p = get_prime(k);

        /* Extend the current factorial from prev_prime+1 up to p */
        idx_t start = (idx_t)prev_prime + 1;
        idx_t end   = (idx_t)p;
        for (idx_t i = start; i <= end; ++i)
            fact *= i;

        /* Update the overall product */
        product *= fact;

        /* Store the result */
        memo[(size_t)k] = product;

        /* Update previous prime for next iteration */
        prev_prime = p;
    }

    last_n = n;
    return memo[(size_t)n];
}
