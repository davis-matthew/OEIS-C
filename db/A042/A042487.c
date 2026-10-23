/*
 * A042487  Denominators of continued fraction convergents to sqrt(771).
 * https://oeis.org/A042487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: The sequence is the denominators q_n of the convergents
 * of the continued‑fraction expansion of √771.
 *
 * Recurrence used:  q_(-1)=0, q_0=1, and for n≥1
 *      q_n = a_n * q_(n-1) + q_(n-2)
 * where a_n are the terms of the continued fraction of √771
 * (a_0 = ⌊√771⌋ = 27, a_1 = 1, ...).
 *
 * The code generates the a_n on the fly using the standard
 * recurrence for quadratic surds and memoises all q_n up to the
 * largest requested index.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static cache of computed denominators */
static seq_t *den_cache = NULL;
static size_t cache_size = 0;   /* number of valid entries */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Continued–fraction state for sqrt(771) */
static uint64_t cf_m = 0;       /* m_n */
static uint64_t cf_d = 1;       /* d_n */
static uint64_t cf_a = 27;      /* a_n  (a_0) */

/* Constants for the root */
static const uint64_t D  = 771; /* radicand */
static const uint64_t A0 = 27;  /* floor(sqrt(D)) */

/* Ensure capacity for at least n+1 cached entries. */
static void ensure_capacity(size_t n) {
    if (cache_cap > n)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= n)
        new_cap <<= 1;
    den_cache = (seq_t *)realloc(den_cache, sizeof(seq_t) * new_cap);
    cache_cap = new_cap;
}

/* Initialise the cache with the first two terms and the CF state
 * after the first coefficient a_1. */
static void init_cache(void) {
    if (cache_size != 0)
        return;                   /* already initialised */

    ensure_capacity(1);
    den_cache[0] = (seq_t)1;       /* q_0 */
    den_cache[1] = (seq_t)1;       /* q_1 (a_1 = 1) */
    cache_size  = 2;

    /* Compute the first continued‑fraction coefficient a_1
     * (which has already been used to obtain q_1) and set the
     * state ready for the next coefficient. */
    uint64_t m1 = cf_d * cf_a - cf_m;                     /* 27 */
    uint64_t d1 = (D - m1 * m1) / cf_d;                 /* 42 */
    uint64_t a1 = (A0 + m1) / d1;                       /* 1 */

    cf_m = m1;
    cf_d = d1;
    cf_a = a1;
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n) {
    if (n == (idx_t)-1)          /* avoid side effect of unsigned wrap‑around */
        return (seq_t)0;

    size_t idx = (size_t)n;

    /* Initialise only once. */
    if (cache_size == 0)
        init_cache();

    /* Make sure the cache can hold the required index. */
    ensure_capacity(idx);

    /* Generate terms up to the requested index if necessary. */
    while (cache_size <= idx) {
        /* Compute the next CF coefficient a_{k+1}. */
        uint64_t m_next = cf_d * cf_a - cf_m;
        uint64_t d_next = (D - m_next * m_next) / cf_d;
        uint64_t a_next = (A0 + m_next) / d_next;

        /* Compute the next denominator: q_{k+1} = a_next·q_k + q_{k-1}. */
        seq_t q = ((seq_t)a_next) * den_cache[cache_size - 1]
                + den_cache[cache_size - 2];
        den_cache[cache_size] = q;

        /* Advance the CF state for the next iteration. */
        cf_m = m_next;
        cf_d = d_next;
        cf_a = a_next;

        ++cache_size;
    }

    return den_cache[idx];
}
