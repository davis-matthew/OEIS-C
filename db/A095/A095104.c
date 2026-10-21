/*
 * A095104  Diving index of the n-th 4k+3 prime (A002145(n)).
 * https://oeis.org/A095104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   ensure_capacity, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = diving index of the n‑th prime ≡ 3 (mod 4);
   computed by searching for the first u > 1 with
   Σ_{i=1..u} J(i/p) = ‑1, where J is the Jacobi symbol.
   If no such u exists, returns 0. */

/* Uses the provided jacobi_symbol util. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/ensure_capacity.h"

/* Static state for prime generation and memoisation. */
static idx_t       *all_primes      = NULL;
static size_t      all_count       = 0;          /* number of primes stored   */
static size_t      all_capacity    = 0;          /* used for reallocation     */

static idx_t       *p3_primes      = NULL;       /* primes ≡ 3 (mod 4) */
static size_t      p3_count       = 0;
static size_t      p3_capacity    = 0;

static seq_t       *dives         = NULL;       /* diving indices       */
static size_t      dive_count     = 0;
static size_t      dive_capacity  = 0;

static idx_t       next_candidate = 3;          /* next odd number to test */

/* Integer square root, simple but sufficient for the small numbers we test. */
static idx_t isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) r++;
    return r;
}

/* Trial division primality test using the primes found so far. */
static bool is_prime(idx_t m)
{
    if (m < 2) return false;
    idx_t root = isqrt(m);
    for (size_t i = 0; i < all_count; ++i) {
        idx_t p = all_primes[i];
        if (p > root) break;
        if (m % p == 0) return false;
    }
    return true;
}

/* Computes the diving index for a 4k+3 prime p. */
static seq_t compute_dive(idx_t p)
{
    long long sum = 0;          /* cumulative Legendre sum; range fits in signed 64‑bit */
    for (idx_t i = 1; i < p; ++i) {
        int ls = jacobi_symbol((uint64_t)i, (uint64_t)p); /* Legendre symbol */
        sum += ls;
        if (sum == -1) {
            return (seq_t)i;   /* cast to seq_t automatically */
        }
    }
    return (seq_t)0;            /* never reached -1 */
}

/* Generate and memoise the first n 4k+3 primes and their diving indices. */
static void prepare(idx_t n)
{
    while (p3_count < (size_t)n) {
        /* Search for the next prime. */
        while (true) {
            if (is_prime(next_candidate)) {
                /* Store the new prime in the complete prime list. */
                ensure_capacity(all_count + 1, &all_capacity,
                                (void **)&all_primes, sizeof(idx_t));
                all_primes[all_count++] = next_candidate;

                /* If it is 4k+3, record it and compute its diving index. */
                if (next_candidate % 4 == 3) {
                    ensure_capacity(p3_count + 1, &p3_capacity,
                                    (void **)&p3_primes, sizeof(idx_t));
                    p3_primes[p3_count++] = next_candidate;

                    ensure_capacity(dive_count + 1, &dive_capacity,
                                    (void **)&dives, sizeof(seq_t));
                    dives[dive_count++] = compute_dive(next_candidate);
                }
                next_candidate += 2;   /* advance to the next odd candidate */
                break;
            }
            next_candidate += 2;       /* skip even numbers */
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    prepare(n);
    return dives[n - 1];
}
