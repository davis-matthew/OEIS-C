/*
 * A042085  Denominators of continued fraction convergents to sqrt(566).
 * https://oeis.org/A042085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 191218570*a(n-18) - a(n-36)  (n > 35)
   Initial values (a(0) … a(35)) are the denominators of the
   continued‑fraction convergents to sqrt(566).  The first 36
   values are generated once from the continued fraction data. */

#include <stdint.h>
#include <stdlib.h>

/* static storage for the cache of computed terms */
static seq_t *cache      = NULL;   /* array of terms a(0)…a(k) */
static idx_t cache_filled = 0;     /* number of terms stored so far */
static size_t cache_cap    = 0;     /* allocated capacity (in terms) */
static int   initialized  = 0;     /* 0 before first call, 1 afterwards */

/* enlarge cache so that it contains indices 0..need inclusive */
static void ensure_capacity(idx_t need)
{
    if (cache_cap <= (size_t)need) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap <= (size_t)need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* compute the first 36 denominators from the continued fraction of sqrt(566) */
static void init_first_terms(void)
{
    /* continued‑fraction data for sqrt(566) (period 18) */
    const unsigned int D = 566;
    unsigned int a0 = 0;
    while ((a0 + 1U) * (a0 + 1U) <= D) ++a0;   /* floor(sqrt(D)) */
    unsigned int m = 0;
    unsigned int d = 1;
    unsigned int a = a0;
    unsigned int a_seq[36];
    for (int k = 0; k < 36; ++k) {
        a_seq[k] = a;
        unsigned int m_new = d * a - m;
        unsigned int d_new = (D - m_new * m_new) / d;
        unsigned int a_new = (a0 + m_new) / d_new;
        m = m_new;
        d = d_new;
        a = a_new;
    }

    /* recurrence for convergents' denominators q_k */
    seq_t p_nm2 = 0; /* p_{-2} */
    seq_t p_nm1 = 1; /* p_{-1} */
    seq_t q_nm2 = 1; /* q_{-2} */
    seq_t q_nm1 = 0; /* q_{-1} */
    for (int k = 0; k < 36; ++k) {
        seq_t p = (seq_t)a_seq[k] * p_nm1 + p_nm2;
        seq_t q = (seq_t)a_seq[k] * q_nm1 + q_nm2;
        cache[k] = q;               /* a(k) = q_k */
        p_nm2 = p_nm1;
        p_nm1 = p;
        q_nm2 = q_nm1;
        q_nm1 = q;
    }
    cache_filled = 36;
    initialized = 1;
}

/* exported function producing the n‑th term of the sequence */
seq_t a(idx_t n)
{
    /* one‑time allocation & initialization */
    if (!initialized) {
        cache = (seq_t *)malloc(64 * sizeof(seq_t));
        if (!cache) abort();          /* out of memory */
        cache_cap = 64;
        init_first_terms();
    }

    /* if the value is already known */
    if (n < cache_filled) return cache[n];

    /* enlarge cache if required */
    ensure_capacity(n);

    /* fill any missing entries (the recurrence can be used for any n) */
    for (idx_t i = cache_filled; i <= n; ++i) {
        if (i < 36) continue;        /* already precomputed */
        cache[i] = (seq_t)191218570 * cache[i - 18] - cache[i - 36];
    }
    cache_filled = n + 1;
    return cache[n];
}
