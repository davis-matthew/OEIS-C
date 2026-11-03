/*
 * A084547  Numbers k that have primitive roots less than k that are
 *          nonnegative perfect cubes.
 * https://oeis.org/A084547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 5058, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9253 (2026-11-01)
 * Helpers:   factor_u64, gcd, isqrt, powmod, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084547: numbers k that have a primitive root less than k that is a non‑negative perfect cube.
   Computed by incremental search. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/isqrt.h"
#include "../utils/powmod.h"
#include "../utils/totient_seq.h"

/* integer cube root: largest k with k³ ≤ x */
static idx_t int_cube_root(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *seq_cache      = NULL;   /* memoised sequence values   */
static size_t cache_len      = 0;       /* number of values stored    */
static size_t cache_cap      = 0;       /* capacity of seq_cache      */
static uint64_t last_checked_k = 1;     /* last k processed           */

/* ---------------------------------------------------------------------- */
/*  Check whether an integer k has primitive roots.
 *  Returns true for k=2,4 or k=p^a or k=2·p^a with p odd prime.      */
static bool has_primitive_root(uint64_t k)
{
    if (k == 2 || k == 4) return true;

    uint64_t pr[20], ex[20];
    size_t   cnt = 0;
    factor_u64(k, pr, ex, &cnt);          /* factorisation of k */

    if (cnt == 1)                      /* k = p^a                 */
        return true;
    if (cnt == 2 && pr[0] == 2 && ex[0] == 1) /* k = 2·p^a, p odd  */
        return true;
    return false;
}

/* ---------------------------------------------------------------------- */
/*  Test if k has a primitive root that is a perfect cube.                  */
static bool has_cube_root_primitive(uint64_t k)
{
    seq_t m   = (seq_t)k;
    seq_t phi_seq = totient_seq(m);
    uint64_t phi = (uint64_t)phi_seq;     /* phi(k) fits in 64 bits for the values we examine */

    uint64_t prphi[20], expphi[20];
    size_t   cnt_phi = 0;
    factor_u64(phi, prphi, expphi, &cnt_phi);

    uint64_t tmax = int_cube_root(k-1);
    for (uint64_t t = 1; t <= tmax; ++t) {
        uint64_t c = t*t*t;
        seq_t cs = (seq_t)c;
        seq_t ks = (seq_t)k;
        if (gcd(cs, ks) != (seq_t)1) continue;   /* must be coprime to k */

        bool is_primitive = true;
        for (size_t i = 0; i < cnt_phi; ++i) {
            uint64_t q   = prphi[i];
            uint64_t exp = phi / q;
            seq_t val = powmod(cs, exp, ks);
            if (val == (seq_t)1) {                /* c is not primitive */
                is_primitive = false;
                break;
            }
        }
        if (is_primitive) return true;
    }
    return false;
}

/* ---------------------------------------------------------------------- */
/*  Ensure the cache has space for at least n elements.                     */
static void grow_cache_for(size_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n) new_cap *= 2;

    seq_cache = seq_realloc(seq_cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* ---------------------------------------------------------------------- */
/*  Main sequence function.                                                 */
seq_t a(idx_t n)
{
    if (n == 0) abort();                    /* offset is 1, index 0 invalid */

    grow_cache_for((size_t)n);
    while (cache_len < (size_t)n) {
        ++last_checked_k;                   /* next candidate k */
        if (!has_primitive_root(last_checked_k)) continue;
        if (has_cube_root_primitive(last_checked_k)) {
            seq_cache[cache_len++] = (seq_t)last_checked_k;
        }
    }
    return seq_cache[(size_t)(n - 1)];
}
