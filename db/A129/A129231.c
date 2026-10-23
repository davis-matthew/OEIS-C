/*
 * A129231  a(n) = floor(n*r) - a(n-1), where r = sqrt(2) and a(0) = 0.
 * https://oeis.org/A129231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(n) = floor(n*sqrt(2)) - a(n-1) for all n ≥ 1
 * Range:     u8 n <= 360, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses an integer approximation of sqrt(2) to avoid
 * floating‑point arithmetic and memoises computed values for O(1) lookup. */

#include <stdlib.h>
#include <stdint.h>

/* Memoisation storage */
static seq_t *cache = NULL;      /* array of stored terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms filled (0 … cache_len–1) */

/* Ensure the cache can hold indices up to `n` (inclusive).  The cast on
 * realloc is required for C++ compatibility. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;  /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)n + 1;
    if (new_cap <= (size_t)n + 1)
        new_cap = (size_t)n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();          /* out of memory – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* Integer approximation of floor(n*sqrt(2)):
 *      sqrt(2) ≈ 1414213562373095048 / 10^18
 * This gives the correct floor for all n up to at least 10^18.  The
 * multiplication is performed in unsigned __int128 to avoid overflow. */
static inline seq_t floor_n_sqrt2(idx_t n)
{
    /* 1414213562373095048 = floor(sqrt(2) * 10^18) */
    static const uint64_t NUM = 1414213562373095048ULL;
    static const uint64_t DEN = 1000000000000000000ULL;
    unsigned __int128 prod = (unsigned __int128)n * NUM;
    uint64_t value = (uint64_t)(prod / DEN);
    return (seq_t)value;
}

/* OEIS sequence A129231 */
seq_t a(idx_t n)
{
    if ((size_t)n < cache_len)
        return cache[n];          /* already computed */

    ensure_cache(n);               /* make room for new terms */

    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t val;
        if (k == 0) {
            val = 0;               /* a(0) = 0 */
        } else {
            seq_t fn = floor_n_sqrt2(k);   /* floor(k*sqrt(2)) */
            val = fn - cache[k-1];         /* recurrence */
        }
        cache[k] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
