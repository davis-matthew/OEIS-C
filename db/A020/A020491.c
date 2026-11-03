/*
 * A020491  Numbers k such that sigma_0(k) divides phi(k).
 * https://oeis.org/A020491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 186, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 88 terms through n = 4760 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A020491: numbers k such that σ0(k) divides φ(k).
   The implementation builds the sequence lazily by testing each integer
   in increasing order until the required nth term is found.  No
   pre‑computed tables or large sieves are used; simple trial division
   provides all needed facts.  This guarantees correctness for any
   seq_t implementation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute d(k) = number of divisors and φ(k) = Euler's totient for k > 0.
   The results fit in uint64_t for all k visited by this program. */
static void divisor_and_phi(uint64_t k, uint64_t *d_out, uint64_t *phi_out)
{
    if (k == 1) {
        *d_out    = 1;
        *phi_out  = 1;
        return;
    }
    uint64_t d = 1;
    uint64_t phi = k;
    uint64_t n = k;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            d *= (exp + 1);
            phi = phi / p * (p - 1);
        }
    }
    if (n > 1) {                 /* remaining prime factor */
        d *= 2;
        phi = phi / n * (n - 1);
    }
    *d_out   = d;
    *phi_out = phi;
}

/* Cache of previously found qualifying numbers (1‑indexed). */
static seq_t *cache  = NULL;        /* array of found values          */
static size_t cache_len = 0;        /* current number of stored terms */
static uint64_t last_k = 0;         /* last integer examined          */

/* Append a new term to the cache, growing it if necessary. */
static void cache_append(uint64_t val)
{
    if (cache_len == 0) {
        size_t cap = 64;                /* initial capacity              */
        cache = mem_new(seq_t, cap);
        if (!cache) abort();
        cache_len = 0;
    }
    /* Grow cache when needed. */
    if (cache_len >= cache_len) {
        size_t new_cap = cache_len ? cache_len * 2 : 64;
        seq_t *newp = mem_resize(seq_t, cache, new_cap);
        if (!newp) abort();
        cache = newp;
    }
    cache[cache_len++] = (seq_t)val;    /* cast to the build's seq_t type   */
}

/* Public interface; returns the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                     /* undefined for n<1 */
    while (cache_len < (size_t)n) {                 /* generate until enough terms */
        ++last_k;
        uint64_t d, phi;
        divisor_and_phi(last_k, &d, &phi);
        if (phi % d == 0) {
            cache_append(last_k);
        }
    }
    return cache[n - 1];
}
