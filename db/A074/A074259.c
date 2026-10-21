/*
 * A074259  Gaps between primes p such that 2p+1 is also prime, i.e.,
 *          Sophie-Germain primes A005384.
 * https://oeis.org/A074259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Gap between successive Sophie‑Germain primes. a(1) = S_2 – S_1,
 *            a(n) = S_{n+1} – S_n, where S_k is the k‑th prime p such that
 *            2p+1 is also prime
 * Range:     u8 n <= 83, u16 n <= 7745, u32 n <= 7745, u64 n <= 7745,
 *            u128 n <= 7745, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides: bool is_prime(idx_t n); */

/* Static cache of Sophie‑Germain primes. */
static idx_t *sgprimes   = NULL;   /* array of SG primes (p values) */
static size_t sg_size    = 0;       /* number of primes stored */
static size_t sg_cap     = 0;       /* current capacity of the array */
static idx_t  next_candidate = 0;   /* smallest integer to test next */

/* Grow the cache to accommodate at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (sg_cap >= new_cap) return;
    size_t cap = sg_cap ? sg_cap * 2 : 8;
    while (cap < new_cap) cap *= 2;
    idx_t *tmp = (idx_t *)realloc(sgprimes, cap * sizeof(idx_t));
    if (!tmp) abort();          /* out of memory */
    sgprimes = tmp;
    sg_cap   = cap;
}

/* Ensure that at least need Sophie‑Germain primes are stored. */
static void ensure_sg_primes(size_t need)
{
    if (sg_size >= need) return;

    /* Initialize candidate if first call. */
    if (sg_size == 0) next_candidate = 2;
    else if (next_candidate <= sgprimes[sg_size - 1]) next_candidate = sgprimes[sg_size - 1] + 1;

    for (;;) {
        /* Skip non‑prime candidates. */
        while (!is_prime(next_candidate)) ++next_candidate;

        idx_t p = next_candidate;
        if (is_prime(2 * p + 1)) {
            /* Store the new Sophie‑Germain prime. */
            grow_cache(sg_size + 1);
            sgprimes[sg_size++] = p;
            if (sg_size >= need) break;
        }
        ++next_candidate;   /* move to the next integer */
    }
}

/* a(idx_t n) – A074259: gap between the n‑th and (n+1)-st Sophie‑Germain primes. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* value below offset; unused */

    /* Need the first (n+1) SG primes to have indices 0..n. */
    ensure_sg_primes((size_t)n + 1);

    /* Difference between consecutive SG primes. */
    seq_t diff = (seq_t)(sgprimes[n] - sgprimes[n - 1]);
    return diff;
}
