/*
 * A210845  Values n for which A055034(n) is squarefree.
 * https://oeis.org/A210845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n such that A055034(n) is squarefree. The degree δ(n) of
 *            the minimal polynomial of 2·cos(π/n) is δ(n) = 1 for n = 1, and
 *            δ(n) = φ(2·n)/2 for n > 1, where φ is Euler’s totient. We
 *            identify those n for which δ(n) is square‑free and cache the
 *            results
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   factor_u64, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/is_squarefree.h"

static seq_t *a_cache   = NULL;   /* cached sequence terms */
static size_t cache_size  = 0;    /* number of terms cached */
static size_t cache_cap   = 0;    /* allocated capacity of cache */
static idx_t last_checked = 1;   /* next integer to examine */

/* --- Utility: grow cache if needed ------------------------------------ */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    /* realloc returns void*, must cast for C++ */
    a_cache = (seq_t*)realloc(a_cache,
                              new_cap * sizeof(seq_t));
    if (!a_cache)
        abort();
    cache_cap = new_cap;
}

/* --- Compute δ(m) = A055034(m) ------------------------------------- */
static seq_t compute_delta(idx_t m)
{
    if (m == 1)
        return (seq_t)1;                /* δ(1) = 1 */
    uint64_t two_m = (uint64_t)m * 2ULL;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(two_m, primes, exps, &cnt);
    uint64_t phi = two_m;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        phi = phi / p * (p - 1ULL);
    }
    return (seq_t)(phi / 2ULL);        /* δ(m) = φ(2m) / 2 */
}

/* --- Public function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n = 0 */

    /* Generate terms until at least n are cached */
    while (cache_size < (size_t)n) {
        seq_t delta = compute_delta(last_checked);
        if (is_squarefree(delta)) {
            ensure_capacity(cache_size + 1);
            a_cache[cache_size++] = (seq_t)last_checked;
        }
        ++last_checked;
    }
    return a_cache[n - 1];
}
