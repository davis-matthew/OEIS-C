/*
 * A023883  Nonprimes whose average of divisors is an integer.
 * https://oeis.org/A023883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, sigma_tau
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A023883: Nonprimes whose average of divisors (σ(n)/τ(n)) is an integer.
   Computation: iterate integers, skip primes, compute σ(n) and τ(n);
   include n in the cache if σ(n) % τ(n) == 0.  Results are cached for
   subsequent calls in any order.  Suitable for seq_t up to 64‑bit and
   arbitrary‑precision as provided by the build. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"         /* provides bool is_prime(idx_t) */
#include "../utils/sigma_tau.h"

/* external integer types supplied by the build                                                      */
extern seq_t *seq_alloc(size_t n);     /* Needed only for realloc; could use stdlib */

static seq_t *cache = NULL;    /* array of found terms of the sequence          */
static size_t cap   = 0;        /* capacity of cache                           */
static size_t len   = 0;        /* number of terms stored so far               */
static uint64_t next = 1;       /* next integer to test (1‑based)              */

/* make sure the cache can hold at least 'needed' terms */
static void ensure_capacity(size_t needed)
{
    if (needed <= cap) return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;
    cap = new_cap;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, safe fallback */

    idx_t target_idx = n - 1;      /* convert to 0‑based index */

    while (len <= target_idx) {
        uint64_t m = next++;
        /* skip primes (except 1) */
        if (m > 1 && is_prime((idx_t)m)) continue;

        seq_t sigma, tau;
        sigma_tau(m, &sigma, &tau);
        if (sigma % tau == 0) {
            ensure_capacity(len + 1);
            cache[len++] = (seq_t)m;
        }
    }

    return cache[target_idx];
}
