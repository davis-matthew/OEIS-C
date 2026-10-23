/*
 * A125614  (Sum of the squares of the quadratic residues of prime(n)) /
 *          prime(n).
 * https://oeis.org/A125614
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (sum of squares of quadratic residues of prime(n))) /
 *            prime(n)
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 430, u64 n <= 1003,
 *            u128 n <= 1003, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate residues for each prime, store results in a static cache. */

#include <stdlib.h>
#include <string.h>
#include "../utils/get_prime.h"  /* provides uint64_t get_prime(idx_t k); */

static seq_t *seq_cache = NULL;    /* cache of computed a(n) values */
static size_t cache_cap = 0;       /* capacity of seq_cache */
static idx_t cached_until = 3;     /* last index computed (offset is 4) */

static uint8_t *visited = NULL;    /* array marking seen residues */
static size_t visited_cap = 0;     /* capacity of visited array */

/* Compute a single term for index n (n >= 4). */
static seq_t compute_term(idx_t n)
{
    uint64_t p = get_prime(n);            /* the nth prime, p > 5 for n >= 4 */
    size_t psize = (size_t)p;            /* size of residue table */

    /* Resize visited array if needed */
    if (psize > visited_cap) {
        uint8_t *newbuf = (uint8_t *)realloc(visited, psize);
        if (!newbuf) exit(EXIT_FAILURE);
        visited = newbuf;
        visited_cap = psize;
    }
    memset(visited, 0, psize);           /* clear previous marks */

    seq_t sum = 0;
    /* enumerate x from 1 to p-1; each residue appears twice, so we
       accumulate only once and later divide by p. */
    for (idx_t x = 1; x < (idx_t)p; ++x) {
        uint64_t t = (x * x) % p;         /* residue q */
        if (!visited[(size_t)t]) {
            visited[(size_t)t] = 1;
            seq_t sq = (seq_t)t * t;
            sum += sq;
        }
    }
    /* sum now holds sum of distinct q^2; divide by p.   */
    return sum / (seq_t)p;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0; /* behavior below offset is irrelevant */

    /* Ensure cache can hold up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *newbuf = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
        if (!newbuf) exit(EXIT_FAILURE);
        seq_cache = newbuf;
        cache_cap = newcap;
    }

    /* Compute missing terms on-demand */
    for (idx_t i = cached_until + 1; i <= n; ++i) {
        seq_cache[i] = compute_term(i);
    }
    cached_until = n;

    return seq_cache[n];
}
