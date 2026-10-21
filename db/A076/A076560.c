/*
 * A076560  a(1)=1; a(n>1)= greatest prime divisor of (a(n-1) + n).
 * https://oeis.org/A076560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 114, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1)=1; for n>1, a(n)=greatest prime divisor of (a(n-1)+n)
 *
 * The function memoises the sequence in a static array.  Each entry is
 * computed by adding the index to the previous term and then extracting
 * its largest prime factor via simple trial division.  All arithmetic
 * is performed with the type seq_t, so the implementation works for
 * any unsigned integer size (uint8_t … unsigned __int128) or for an
 * arbitrary‑precision integer class used by the build.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* cache for previously computed terms (1‑based) */
static seq_t *cache      = NULL;  /* dynamic array */
static idx_t cached_n    = 0;     /* largest index currently cached */

/* ensure that the cache can hold values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= cached_n)
        return;

    /* grow the cache multiplicatively */
    size_t new_cap = (size_t)(cached_n + 1) * 2;
    while (new_cap < (size_t)(n + 1))
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) {
        /* allocation failed – terminate */
        exit(EXIT_FAILURE);
    }
    cache = tmp;

    /* initialise new slots to zero */
    for (idx_t i = cached_n + 1; i <= n; ++i)
        cache[i] = (seq_t)0;

    cached_n = n;
}

/* largest prime factor of a seq_t value, using trial division */
static seq_t largest_prime_factor(seq_t v)
{
    if (v < (seq_t)2)
        return (seq_t)1;            /* by convention for 0 and 1 */

    seq_t max_pf = (seq_t)1;
    seq_t two   = (seq_t)2;

    /* factor out all 2's first */
    while ((v % two) == (seq_t)0) {
        v /= two;
        max_pf = two;
    }

    /* test odd divisors 3,5,7,… up to sqrt(v) */
    seq_t p = (seq_t)3;
    for (; p * p <= v; p += two) {
        while ((v % p) == (seq_t)0) {
            v /= p;
            max_pf = p;
        }
    }

    /* if anything remains it is prime */
    if (v > (seq_t)1)
        max_pf = v;

    return max_pf;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;       /* undefined for n<1 */

    ensure_cache(n);

    /* already computed? */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* base case */
    if (n == (idx_t)1) {
        cache[1] = (seq_t)1;
        return cache[1];
    }

    /* ensure previous term is ready */
    if (cache[n - (idx_t)1] == (seq_t)0)
        (void)a(n - (idx_t)1);

    /* compute a(n) */
    seq_t sum = cache[n - (idx_t)1] + (seq_t)n;
    cache[n] = largest_prime_factor(sum);
    return cache[n];
}
