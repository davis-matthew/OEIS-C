/*
 * A041743  Denominators of continued fraction convergents to sqrt(391).
 * https://oeis.org/A041743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 14677360*a(n-16) – a(n-32) for n>31
   Initial values (n = 0 … 31):
   1, 1, 4, 9, 22, 31, 53, 137, 2656, 5449, 8105, 13554, 35213,
   83980, 287153, 371133, 14390207, 14761340, 58674227, 132109794,
   322893815, 455003609, 777897424, 2010798457, …
   (indices 0‑31 are computed from the convergents of sqrt(391)). */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for sequence values */
static seq_t *seq_cache = NULL;  /* pointer to cached values          */
static size_t cache_cap = 0;      /* allocated capacity (in elements)  */
static size_t cache_len = 0;      /* number of values stored (size)   */

static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        cache_cap = new_cap;
    }
}

static void precompute_initial_terms(void)
{
    /* Only compute the first 32 terms if not done yet */
    if (cache_len >= 32) return;

    /* Continued‑fraction data for sqrt(391) */
    const uint64_t D  = 391ULL;
    const uint64_t a0 = 19ULL;   /* floor(sqrt(391)) */

    uint64_t m   = 0ULL;
    uint64_t d   = 1ULL;
    uint64_t a_k = a0;           /* current partial quotient */

    /* q_{-1} = 0, q_0 = 1  (denominator of the 0th convergent) */
    seq_t q_prev2 = (seq_t)0;
    seq_t q_prev1 = (seq_t)1;

    seq_cache[0] = (seq_t)1;      /* a(0) */
    cache_len = 1;

    /* Generate convergents up to index 31 (i.e. 32 terms) */
    for (size_t k = 1; k <= 31; ++k) {
        /* Update continued‑fraction data for a_k (k ≥ 1) */
        m = d * a_k - m;
        d = (D - m * m) / d;
        a_k = (a0 + m) / d;

        /* q_k = a_k * q_{k-1} + q_{k-2} */
        seq_t q = (seq_t)a_k * q_prev1 + q_prev2;
        seq_cache[cache_len++] = q;

        q_prev2 = q_prev1;
        q_prev1 = q;
    }
}

seq_t a(idx_t n)
{
    ensure_cache_capacity(n);

    /* Ensure the first 32 terms are available */
    precompute_initial_terms();

    /* If the requested term is already cached, return it */
    if ((size_t)n < cache_len)
        return seq_cache[(size_t)n];

    /* Compute remaining terms using the linear recurrence for n >= 32 */
    /* cache_len should be 32 after precomputation */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)14677360 * seq_cache[i - 16] - seq_cache[i - 32];
        seq_cache[i] = term;
    }

    return seq_cache[(size_t)n];
}
