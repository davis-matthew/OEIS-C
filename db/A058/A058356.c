/*
 * A058356  Coefficients in the series (1 + 2x^2 + 3x^3 + 5x^5 + 7x^7 + 11x^11
 *          + 13x^13 + ... )/(1 - x - 4x^4 - 6x^6 - 8x^8 - 9x^9 - 10x^10 -
 *          12x^12 - 14x^14 - ... ).
 * https://oeis.org/A058356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = N_n + a(n-1) + ∑_{c composite ≤ n} c · a(n-c) * where N_0
 *            = 1, N_p = p for prime p, otherwise N_n = 0. * The coefficients
 *            of the generating function in the OEIS entry * are defined by
 *            the series (1+∑_{p prime} p x^p) / (1−x−∑_{c composite} c x^c)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Cache for computed sequence values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Prime flag array (true → prime, false → composite) */
static bool *prime_flags = NULL;
static idx_t prime_flags_size = 0;

/* Simple sieve of Eratosthenes up to n, storing prime flags.  */
static void ensure_primes(idx_t n)
{
    if (prime_flags_size >= n + 1) return;  /* already computed */

    bool *newflags = (bool *)realloc(prime_flags, (n + 1) * sizeof(bool));
    if (!newflags) abort();                 /* allocation failed */
    prime_flags = newflags;

    for (idx_t i = 0; i <= n; ++i) prime_flags[i] = true;
    if (n >= 0) prime_flags[0] = false;
    if (n >= 1) prime_flags[1] = false;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (prime_flags[p]) {
            for (idx_t m = p * p; m <= n; m += p)
                prime_flags[m] = false;
        }
    }
    prime_flags_size = n + 1;
}

/* Return the nth term of A058356.  */
seq_t a(idx_t n)
{
    /* Initialise cache for a(0). */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;  /* a(0) = 1 (N_0) */
        cache_size = 1;
    }

    /* Extend cache up to n. */
    if (n >= cache_size) {
        seq_t *newcache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!newcache) abort();
        cache = newcache;

        /* Make sure prime flags are available up to n. */
        ensure_primes(n);

        /* Compute new terms a(cache_size … n) iteratively. */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;

            /* a(i-1) term */
            if (i >= 1) sum += cache[i - 1];

            /* Composite contributions c·a(i-c) */
            for (idx_t c = 2; c <= i; ++c) {
                if (!prime_flags[c]) {
                    sum += (seq_t)c * cache[i - c];
                }
            }

            /* Prime contribution N_i = i if i is prime. */
            if (prime_flags[i]) sum += (seq_t)i;

            cache[i] = sum;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
