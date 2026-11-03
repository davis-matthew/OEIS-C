/*
 * A095104  Diving index of the n-th 4k+3 prime (A002145(n)).
 * https://oeis.org/A095104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Validated: partial, pass, 92 terms through n = 92 (2026-11-03)
 * Helpers:   mem
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
#include <stdint.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

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
                                OEISC_VPP(&all_primes), sizeof(idx_t));
                all_primes[all_count++] = next_candidate;

                /* If it is 4k+3, record it and compute its diving index. */
                if (next_candidate % 4 == 3) {
                    ensure_capacity(p3_count + 1, &p3_capacity,
                                    OEISC_VPP(&p3_primes), sizeof(idx_t));
                    p3_primes[p3_count++] = next_candidate;

                    ensure_capacity(dive_count + 1, &dive_capacity,
                                    OEISC_VPP(&dives), sizeof(seq_t));
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
