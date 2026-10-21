/*
 * A216075  Numbers n such that 2*prime(n) - prime(n+1) is prime.
 * https://oeis.org/A216075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 576, u32 n <= 577, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A216075:  Numbers n such that 2*prime(n) - prime(n+1) is prime
 *  a(n) is the n‑th such integer (a(1)=3, a(2)=4, …).
 *  The implementation builds a table of primes once and caches the
 *  sequence values.  It works for arbitrary‑precision seq_t, uint64_t,
 *  and other supported integer types.  The code respects the
 *  requirements for C11 and C++17 without using <new> or other
 *  non‑portable constructs.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t)             */
#include "../utils/build_primes_upto.h"/* build_primes_upto(idx_t limit, ... ) */

/* --- Static state ---------------------------------------------------- */
static uint64_t *prime_values = NULL;   /* prime list, 1‑based: prime_values[0]=2   */
static size_t   prime_count    = 0;      /* number of primes stored                */
static idx_t    prime_limit    = 1000000;/* current upper bound passed to build_primes_upto */

static seq_t *seq_cache   = NULL;        /* cached sequence values */
static size_t seq_len     = 0;           /* number of terms stored   */
static size_t seq_cap     = 0;           /* allocated capacity       */

/* last candidate n that has been checked (0 means none checked yet) */
static idx_t checked_n = 0;

/* --------------------------------------------------------------------- */
static void init_primes(void)
{
    /* build primes up to the initial limit */
    build_primes_upto(prime_limit, &prime_values, &prime_count);
}

/* Ensure we have at least (needed + 1) primes (indices 1 … needed+1) */
static void ensure_primes(idx_t needed)
{
    if (prime_count > (size_t)needed)
        return;
    /* Double the limit until it is sufficient. */
    while (prime_count <= (size_t)needed)
    {
        prime_limit *= 2;
        build_primes_upto(prime_limit, &prime_values, &prime_count);
    }
}

/* Grow the sequence cache to hold at least n terms (1‑based). */
static void grow_cache(size_t n)
{
    if (n <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    while (new_cap < n)
        new_cap <<= 1;
    /* realloc is allowed in C++ after a C‑style cast */
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Generate terms up to n.  After this, seq_cache contains at least n terms. */
static void compute_up_to(idx_t n)
{
    if (!prime_values)
        init_primes();

    grow_cache((size_t)n);

    while (seq_len < (size_t)n)
    {
        ++checked_n;                    /* candidate n (1‑based) */
        /* Ensure primes up to checked_n+1 exist */
        ensure_primes(checked_n);

        /* access primes[checked_n-1] == prime(checked_n)
           and primes[checked_n]   == prime(checked_n+1)            */
        seq_t p = (seq_t)prime_values[checked_n - 1];
        seq_t q = (seq_t)prime_values[checked_n];

        /* 2*prime(checked_n) - prime(checked_n+1) */
        seq_t val = (seq_t)(( (seq_t)2 * p ) - q);

        if (is_prime_big(val))
            seq_cache[seq_len++] = (seq_t)checked_n; /* store the integer n */
    }
}

/* --------------------------------------------------------------------- */
/* Public interface required by the build system                      */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined, but harmless */
    compute_up_to(n);
    return seq_cache[n - 1];
}
