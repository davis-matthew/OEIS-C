/*
 * A029707  Numbers k such that the k-th and the (k+1)-st primes are twin
 *          primes.
 * https://oeis.org/A029707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 6906, u32 n <= 9576, u64 n <= 9576,
 *            u128 n <= 9576, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sieve primes until we have at least n twin‑prime pairs,
 * storing the indices of the first prime of each twin pair.
 * a(n) returns the index k such that the k‑th and (k+1)‑st primes are
 * twin primes.  The sequence is indexed from 1 (a(1)=2).  Values are
 * returned as seq_t; the internal index is kept as idx_t. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes.h"

/* Static cache of twin‑prime indices: 1‑based indices of the first prime
 * in each twin pair, stored as seq_t. */
static seq_t *twin_indices = NULL;
static size_t twin_count   = 0;

/* Current sieve limit.  Starts modest and is doubled as needed. */
static idx_t current_limit = 200000;   /* 200 000 is enough for about 1 400 twin pairs */

/* Rebuild the twin‑prime table up to the given sieve limit. */
static void rebuild_twin_indices(idx_t limit)
{
    idx_t *primes      = NULL;
    size_t prime_count = 0;
    build_primes(limit, &primes, &prime_count);

    /* Count twin pairs. */
    size_t cnt = 0;
    for (size_t i = 0; i + 1 < prime_count; ++i)
        if (primes[i + 1] == primes[i] + 2u) ++cnt;

    /* Allocate new storage for the indices. */
    seq_t *new_twin = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!new_twin) {
        free(primes);
        exit(EXIT_FAILURE);
    }

    /* Populate the array with 1‑based indices. */
    size_t pos = 0;
    for (size_t i = 0; i + 1 < prime_count; ++i)
        if (primes[i + 1] == primes[i] + 2u)
            new_twin[pos++] = (seq_t)(i + 1);

    /* Swap the new array into place. */
    free(twin_indices);
    twin_indices = new_twin;
    twin_count   = cnt;

    free(primes);
}

/* Ensure that at least `n` twin indices are available.  If not, the
 * sieve limit is doubled until enough are found. */
static void ensure_twin_count(idx_t n)
{
    if (twin_count >= (size_t)n) return;   /* already enough */

    /* If no sieve has been run yet, do an initial run. */
    if (!twin_indices) rebuild_twin_indices(current_limit);

    while (twin_count < (size_t)n) {
        current_limit *= 2;                /* increase the limit */
        rebuild_twin_indices(current_limit);
    }
}

/* Public sequence routine.  The function is called with 1‑based `n`. */
seq_t a(idx_t n)
{
    ensure_twin_count(n);
    return twin_indices[n - 1];
}
