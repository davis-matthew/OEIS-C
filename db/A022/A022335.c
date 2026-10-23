/*
 * A022335  Index of 2^n within sequence of numbers of form 2^i * 5^j.
 * https://oeis.org/A022335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} (1 + floor(log_5(2^i)))
 * Range:     u8 n <= 32, u16 n <= 550, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 64‑bit multiplication used only on idx_t which is always uint64_t.
 * The product i*MULT stays < 2^63, so it fits safely.
 * SCALE and MULT approximate log_5(2) ≈ 0.430676558073164…
 * The relative error is < 10⁻¹⁰, giving a margin of <10⁻⁶ over
 * all i ≤ 10⁴, so the integer division yields the exact floor. */
#include <stdlib.h>
#include <stdint.h>

/* Constants for floor(log_5(2^i)) calculation */
static const idx_t LOG5_2_MULT   = 430676558ULL;     /* floor(log_5(2) * 1e9)   */
static const idx_t LOG5_2_SCALE  = 1000000000ULL;    /* 1e9                     */

/* Cache for already computed sequence values */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static idx_t max_cached = 0;

/* Ensure the cache can hold values up to index n */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                  /* abort on allocation failure */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* One‑time initialization */
    if (!seq_cache) {
        ensure_cache(0);
        seq_cache[0] = (seq_t)1;   /* a(0) = 1 */
        max_cached   = 0;
    }

    /* If the requested value is already cached, return it */
    if (n <= max_cached)
        return seq_cache[n];

    /* Compute all missing values up to n */
    ensure_cache(n);
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* floor(log_5(2^i))  ≈  (i * MULT) / SCALE */
        idx_t ij = ((idx_t)i * LOG5_2_MULT) / LOG5_2_SCALE;
        seq_t j = (seq_t)ij;
        /* a(i) = a(i-1) + 1 + j */
        seq_cache[i] = seq_cache[i - 1] + (seq_t)1 + j;
    }
    max_cached = n;
    return seq_cache[n];
}
