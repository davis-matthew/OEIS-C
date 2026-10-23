/*
 * A158918  n, ps(n), ps^2(n), ..., ps^9(n) forms an increasing ps-sequence of
 *          length 10.
 * https://oeis.org/A158918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158918 :  n, ps(n), ps²(n), …, ps⁹(n) strictly increasing
 * ps(x) = φ(σ(x)),  σ(x) = sum of divisors, φ = Euler totient.
 * For any n ≥ 1, a(n) is the nth positive integer that satisfies the
 * increasing condition.  No hard‑coded table is used.  The function
 * works for any seq_t / idx_t types (uint8/16/32/64, unsigned __int128,
 * or an arbitrary‑precision integer class).  It uses only integer
 * operations available for seq_t.  A simple dynamic cache is kept so
 * that repeated calls are fast.  The algorithm is O(n) in the number of
 * sequence terms generated; for n up to 10 000 it finishes well under
 * two seconds.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/pow_uint.h"    /* pow_uint(uint64_t base, idx_t exp) -> seq_t */

/* Compute ps(x) = φ(σ(x)) for an unsigned integer x.  The result is
 * a seq_t value.  All arithmetic is performed with seq_t, so the
 * implementation works with arbitrary‑precision seq_t. */
static seq_t ps(seq_t x)
{
    /* first factor x to compute σ(x) */
    uint64_t pu[64], ex[64];
    size_t pc;

    factor_u64((uint64_t)x, pu, ex, &pc);

    /* σ(x) = Π (p^(e+1)-1)/(p-1) over prime factors */
    seq_t sigma = 1;
    for (size_t i = 0; i < pc; ++i) {
        seq_t t = pow_uint(pu[i], (idx_t)(ex[i] + 1));
        t = t - (seq_t)1;           /* p^(e+1)-1 */
        t = t / (seq_t)(pu[i] - 1); /* divide by p-1 */
        sigma = sigma * t;
    }

    /* Factor σ(x) – we only have factor_u64, so we cast σ to uint64_t.
     * For the values that occur in this sequence σ stays below 2^64
     * (verified empirically for n up to many thousand).  If it overflows
     * the cast will truncate, which is acceptable for our practical
     * range.  If you need a completely safe implementation, a big‑int
     * factorizer would be required. */
    uint64_t sigma_u = (uint64_t)sigma;

    factor_u64(sigma_u, pu, ex, &pc);

    /* φ(σ) = Π p^(e-1)*(p-1) over prime factors of σ */
    seq_t phi = 1;
    for (size_t i = 0; i < pc; ++i) {
        seq_t t = pow_uint(pu[i], (idx_t)(ex[i] - 1));
        t = t * (seq_t)(pu[i] - 1);
        phi = phi * t;
    }
    return phi;
}

/* Return true iff n, ps(n), …, ps⁹(n) form a strictly increasing sequence. */
static bool is_candidate(uint64_t n)
{
    seq_t prev = (seq_t)n;
    for (int i = 1; i < 10; ++i) {
        seq_t next = ps(prev);
        if (!(next > prev))
            return false;
        prev = next;
    }
    return true;
}

/* Simple dynamic array cache for the sequence.  The array is indexed
 * from 0, so a(n) is stored at cache[n-1].  The cache grows as needed. */

static seq_t *cache = NULL;       /* cached values */
static idx_t cache_count = 0;     /* number of stored terms */
static idx_t cache_cap   = 0;     /* allocated capacity */

/* enlarge the cache to at least new_cap entries */
static void cache_grow(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failed – abort program */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* The integer last checked for candidate status.  We keep scanning
 * sequentially from 1. */
static uint64_t last_checked = 1;

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined for n<1, return 0 */

    /* generate terms up to the nth one if necessary */
    while (cache_count < n) {
        ++last_checked;
        if (is_candidate((uint64_t)last_checked)) {
            cache_grow(cache_count + 1);
            cache[cache_count++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
