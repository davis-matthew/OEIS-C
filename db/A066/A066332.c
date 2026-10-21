/*
 * A066332  a(1)=1; for n > 0, a(n+1) = rad(a(n))*n where rad=A007947.
 * https://oeis.org/A066332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) * primorial(n-2) where primorial(k) = product of
 *            all primes ≤ k (primorial(0) = 1) Equivalent recurrence: a(1) =
 *            1; a(n+1) = rad(a(n)) * n
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of primorials: prim_cache[i] = product of all primes ≤ i */
static seq_t *prim_cache = NULL;
static size_t prim_cap  = 0;

/* Ensure prim_cache contains primorials up to 'limit' (inclusive) */
static void ensure_primorial(idx_t limit)
{
    /* If already computed up to the needed limit, nothing to do */
    if (prim_cache != NULL && prim_cap >= (size_t)limit)
        return;

    /* Allocate space for the new range (indices 0 … limit) */
    size_t new_cap = (size_t)limit + 1;
    seq_t *tmp = (seq_t *)realloc(prim_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* memory allocation failure */
    prim_cache = tmp;

    /* Sieve of Eratosthenes up to 'limit' */
    bool *is_prime = (bool *)malloc(new_cap * sizeof(bool));
    if (!is_prime)
        abort();
    for (size_t i = 0; i < new_cap; ++i)
        is_prime[i] = true;
    if (new_cap > 0)
        is_prime[0] = false;
    if (new_cap > 1)
        is_prime[1] = false;
    for (size_t p = 2; p * p <= (size_t)limit; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= (size_t)limit; m += p)
                is_prime[m] = false;
        }
    }

    /* Compute primorials incrementally */
    prim_cache[0] = (seq_t)1;
    seq_t prod = (seq_t)1;
    for (size_t i = 1; i <= (size_t)limit; ++i) {
        if (is_prime[i])
            prod = prod * (seq_t)i;
        prim_cache[i] = prod;
    }

    prim_cap   = (size_t)limit;
    free(is_prime);
}

/* Sequence function required by the build */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    if (n == (idx_t)1)          /* base case */
        return (seq_t)1;

    idx_t m      = n - 1;        /* factor (n-1) in the formula */
    idx_t limit  = n - 2;        /* primorial of (n-2) */
    ensure_primorial(limit);

    return (seq_t)m * prim_cache[limit];
}
