/*
 * A217856  Numbers with three prime factors, not necessarily distinct, except
 *          cubes of primes.
 * https://oeis.org/A217856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_spf, int_cube_root, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute terms of A217856 (Numbers with three prime factors, not cubes of a prime) by
 * sieving up to a dynamic limit and selecting numbers with Ω(n)=3 that are not p³. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/int_cube_root.h"
#include "../utils/is_prime.h"

static uint64_t *term_buffer = NULL;   /* buffer holding the terms (uint64_t) */
static size_t   buf_len    = 0;        /* number of terms stored currently */
static size_t   buf_cap    = 0;        /* allocated capacity of term_buffer */
static idx_t    sieve_limit = 0;       /* last limit processed by build_sequence */

/* Build the sequence up to `limit`. The function rebuilds the whole cache from scratch. */
static void build_sequence(idx_t limit)
{
    if (limit < 4) limit = 4; /* minimal limit to include small terms */

    /* Smallest‑prime‑factor table (spf) */
    idx_t *spf = NULL;
    build_spf(limit, &spf); /* spf[0]=spf[1]=0, spf[i] gives smallest prime factor of i */

    /* Omega array: number of prime factors with multiplicity */
    uint8_t *omega = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    omega[0] = omega[1] = 0;
    for (idx_t i = 2; i <= limit; ++i)
        omega[i] = omega[i / spf[i]] + 1;

    /* Resize/allocate term buffer if necessary */
    if (buf_cap == 0) {
        buf_cap = 1024;
        term_buffer = (uint64_t *)malloc(buf_cap * sizeof(uint64_t));
    } else if (buf_cap < 1024) {
        buf_cap = 1024;
        term_buffer = (uint64_t *)realloc(term_buffer, buf_cap * sizeof(uint64_t));
    }
    buf_len = 0;

    for (idx_t n = 2; n <= limit; ++n) {
        if (omega[n] != 3) continue;   /* need exactly three prime factors */

        idx_t r = int_cube_root(n);
        if (r * r * r == n && is_prime(r))
            continue;                 /* skip cubes of a single prime */

        /* Grow buffer if needed */
        if (buf_len == buf_cap) {
            buf_cap <<= 1;
            term_buffer = (uint64_t *)realloc(term_buffer, buf_cap * sizeof(uint64_t));
        }
        term_buffer[buf_len++] = (uint64_t)n;
    }

    /* Clean up */
    free(spf);
    free(omega);
    sieve_limit = limit;
}

/* Return the nth term of A217856 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined term */

    /* Ensure enough terms are cached */
    if (buf_len < n) {
        /* Estimate a new sieve limit large enough for at least `n` terms.
           The ratio between n and the number of 3‑factor numbers is roughly 1:200
           for the range up to 10⁵; we use a generous factor of 5. */
        idx_t est = sieve_limit == 0 ? (n * 5 + 100) : (sieve_limit * 2);
        while (buf_len < n) {
            build_sequence(est);
            if (buf_len >= n) break;
            est <<= 1; /* double the limit and retry */
        }
    }

    /* Convert stored uint64_t to seq_t before returning */
    return (seq_t)term_buffer[n - 1];
}
