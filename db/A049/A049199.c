/*
 * A049199  Nonsquarefree numbers whose Euler totient function is also
 *          nonsquarefree.
 * https://oeis.org/A049199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Nonsquarefree numbers k such that k and φ(k) are not squarefree
 *            (computed by a sieve of Eratosthenes and Euler’s totient)
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Upper bound for the sieve – large enough for the first 10 000 terms. */
#define MAX_BOUND 2000000

/* Static state used by a() */
static bool     initialized = false;
static seq_t  *phi          = NULL;   /* φ(n) for 0 … MAX_BOUND */
static uint8_t *nonsq        = NULL;   /* 1 if n is NOT squarefree */
static seq_t  *terms        = NULL;   /* collected sequence numbers */
static size_t  terms_size   = 0;
static size_t  terms_cap    = 0;

/* Ensure the terms array has room for at least need more elements. */
static void grow_terms(void)
{
    size_t new_cap = terms_cap ? terms_cap * 2 : 1024;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);         /* abort on memory failure */
    terms = tmp;
    terms_cap = new_cap;
}

/* Integer square‑root via linear scan (fast enough for MAX_BOUND). */
static idx_t int_isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) r++;
    return r;
}

/* Pre‑compute φ, nonsquarefree flags and the sequence itself. */
static void init_sequence(void)
{
    if (initialized) return;

    /* Allocate working arrays */
    phi   = (seq_t *)malloc((MAX_BOUND + 1) * sizeof(seq_t));
    nonsq = (uint8_t *)calloc((MAX_BOUND + 1), sizeof(uint8_t));

    if (!phi || !nonsq) exit(1);

    /* Initialise φ[n] = n */
    for (idx_t i = 0; i <= MAX_BOUND; ++i) phi[i] = (seq_t)i;

    /* Compute φ by the classic sieve method */
    for (idx_t p = 2; p <= MAX_BOUND; ++p)
    {
        if (phi[p] == (seq_t)p)          /* p is prime at this point */
        {
            for (idx_t j = p; j <= MAX_BOUND; j += p)
            {
                seq_t val = phi[j];
                phi[j] = val - val / p;
            }
        }
    }

    /* Mark numbers that are NOT squarefree: multiples of p² for prime p */
    idx_t maxp = int_isqrt(MAX_BOUND);
    for (idx_t p = 2; p <= maxp; ++p)
    {
        /* After the φ sieve, a prime p satisfies φ[p] == p‑1 */
        if (phi[p] == (seq_t)(p - 1))
        {
            idx_t sq = p * p;
            for (idx_t j = sq; j <= MAX_BOUND; j += sq)
                nonsq[j] = 1;
        }
    }

    /* Collect the sequence terms */
    grow_terms();
    for (idx_t k = 2; k <= MAX_BOUND; ++k)
    {
        if (nonsq[k] && nonsq[(idx_t)phi[k]])
        {
            if (terms_size == terms_cap) grow_terms();
            terms[terms_size++] = (seq_t)k;
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    init_sequence();
    if (n == 0 || n > terms_size) return 0;   /* undefined for out of bounds */
    return terms[n - 1];
}
