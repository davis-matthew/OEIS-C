/*
 * A051352  a(0) = 0; for n>0, a(n) = a(n-1) + n if n not prime else a(n-1) -
 *          n.
 * https://oeis.org/A051352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   not stated
 * Range:     i8 n <= 25, i16 n <= 321, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A051352
   a(0) = 0
   for n > 0:  a(n) = a(n‑1) + n   if n is not prime
                    a(n‑1) – n   if n is prime
   The function caches previously computed values so that calls
   may be in arbitrary order. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool initialized = false;
static idx_t max_n = 0;
static seq_t *a_cache = NULL;
static bool *prime_flag = NULL;

/* Ensure the sequence and prime table are computed up to n */
static void ensure_compute(idx_t n)
{
    if (initialized && n <= max_n)
        return;

    /* allocate new tables */
    bool *new_prime_flag = (bool*)malloc((n + 1) * sizeof(bool));
    seq_t *new_a_cache   = (seq_t*)malloc((n + 1) * sizeof(seq_t));

    if (!new_prime_flag || !new_a_cache) {
        /* allocation failed – terminate the program */
        abort();
    }

    /* sieve of Eratosthenes up to n */
    for (idx_t i = 0; i <= n; ++i)
        new_prime_flag[i] = true;
    if (n >= 0) new_prime_flag[0] = false;
    if (n >= 1) new_prime_flag[1] = false;
    for (idx_t p = 2; (p * p) <= n; ++p) {
        if (new_prime_flag[p]) {
            for (idx_t m = p * p; m <= n; m += p)
                new_prime_flag[m] = false;
        }
    }

    /* compute the sequence values */
    int64_t current = 0;
    new_a_cache[0] = (seq_t)current;
    for (idx_t i = 1; i <= n; ++i) {
        if (new_prime_flag[i])
            current -= (int64_t)i;
        else
            current += (int64_t)i;

        new_a_cache[i] = (seq_t)current;
    }

    /* free old tables */
    free(prime_flag);
    free(a_cache);

    /* store new tables */
    prime_flag = new_prime_flag;
    a_cache   = new_a_cache;
    max_n     = n;
    initialized = true;
}

seq_t a(idx_t n)
{
    ensure_compute(n);
    return a_cache[n];
}
