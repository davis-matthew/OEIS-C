/*
 * A045401  Primes congruent to {0, 3, 5} mod 7.
 * https://oeis.org/A045401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p with p ≡ 0,3,5 (mod 7) (OEIS A045401)
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds an on‑demand cache of the required primes
   using a simple sieve.  It works for arbitrary‑precision seq_t and
   for 64‑bit builds. */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build system. */

static seq_t *desired = NULL;   /* cached primes in the desired residue classes */
static size_t desired_len = 0;  /* number of primes stored */

/* Build (or rebuild) a cache of all primes ≤ limit that satisfy
   p ≡ 0,3,5 (mod 7).  The function is safe to call repeatedly. */
static void build_desired(idx_t limit)
{
    /* Allocate flag array – cast for strict C++ compliance. */
    unsigned char *composite = (unsigned char *)calloc((size_t)limit + 1, 1);
    if (!composite)
        return; /* memory error – caller may retry with a larger limit */

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (!composite[i])
            for (idx_t j = i * i; j <= limit; j += i)
                composite[j] = 1;

    /* Count qualifying primes to size the array. */
    size_t cnt = 0;
    for (idx_t p = 2; p <= limit; ++p)
        if (!composite[p] && ((p % 7) == 0 || (p % 7) == 3 || (p % 7) == 5))
            ++cnt;

    /* Allocate array for the qualifying primes. */
    seq_t *tmp = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!tmp) {
        free(composite);
        return; /* memory error */
    }

    /* Store the qualifying primes. */
    size_t pos = 0;
    for (idx_t p = 2; p <= limit; ++p)
        if (!composite[p] && ((p % 7) == 0 || (p % 7) == 3 || (p % 7) == 5))
            tmp[pos++] = (seq_t)p;

    /* Replace the old cache. */
    free(desired);
    desired      = tmp;
    desired_len  = cnt;

    free(composite);
}

/* Return the nth element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* out‑of‑range guard */

    /* Ensure the cache contains at least n primes of the desired form. */
    if (desired_len < (size_t)n) {
        /* Heuristic upper bound for the sieve limit. */
        idx_t limit = (n < 10) ? 200 : ((idx_t)(n * 20) + 100);

        /* Rebuild until we have enough primes. */
        while (desired_len < (size_t)n) {
            build_desired(limit);
            if (desired_len < (size_t)n)
                limit = limit * 2 + 100;
        }
    }

    /* Return the (n‑1)-th element (1‑based index). */
    return desired[(size_t)n - 1];
}
