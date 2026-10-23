/*
 * A041725  Denominators of continued fraction convergents to sqrt(382).
 * https://oeis.org/A041725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A041725 – denominators of continued‑fraction convergents to sqrt(382).
 * The denomi­nators q_n satisfy
 *    q_0 = 1,      q_1 = 1,
 *    q_n = a_n·q_{n-1} + q_{n-2}  (n ≥ 2),
 * where a_n are the partial quotients of the
 * continued‑fraction expansion of √382.
 * This implementation generates both the a_n and the q_n on demand and caches 
 * all computed values for arbitrary call order.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for computed values */
static seq_t *denoms   = NULL;      /* array of q_n */
static size_t denoms_cap  = 0;      /* allocated capacity of 'denoms' */
static size_t denoms_len  = 0;      /* number of elements currently stored (≥2 after init) */

/* state of the continued‑fraction generator for √382 */
static unsigned long long cf_m = 0;  /* current m_k */
static unsigned long long cf_d = 1;  /* current d_k */
static unsigned long long cf_a = 19; /* current a_k (initial a_0 = 19) */

/* ensures that the array `denoms` has room for indices 0 … n */
static void ensure_denoms_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (denoms_cap < needed) {
        size_t new_cap = denoms_cap ? denoms_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        denoms = (seq_t *)realloc(denoms, new_cap * sizeof(seq_t));
        denoms_cap = new_cap;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* initialise storage on first call */
    if (denoms_len == 0) {
        /* we always keep q_0 and q_1 available */
        ensure_denoms_capacity(1);                     /* capacity for indices 0 and 1 */
        denoms[0] = (seq_t)1;
        denoms[1] = (seq_t)1;
        denoms_len = 2;                                 /* we have 0 and 1 stored */

        /* initialise continued‑fraction state to the value after a_1 */
        cf_m = 0;  cf_d = 1;  cf_a = 19;                /* current state is a_0 */
        /* step to compute a_1 and advance state to a_1 */
        cf_m = cf_d * cf_a - cf_m;                       /* m_1 = d_0*a_0 - m_0 */
        cf_d = (382ULL - cf_m * cf_m) / cf_d;            /* d_1 = (D - m_1²)/d_0 */
        cf_a = (19ULL + cf_m) / cf_d;                    /* a_1 = floor((a_0+m_1)/d_1) */
        /* now `cf_m,cf_d,cf_a` correspond to a_1, ready for the next coefficient a_2 */
    }

    if (n < denoms_len)              /* already known */
        return denoms[n];

    /* grow storage to accommodate up to index n */
    ensure_denoms_capacity(n);

    /* compute missing denominators up to n */
    for (idx_t i = denoms_len; i <= n; ++i) {
        /* advance continued‑fraction state to the next coefficient a_i */
        cf_m = cf_d * cf_a - cf_m;
        cf_d = (382ULL - cf_m * cf_m) / cf_d;
        cf_a = (19ULL + cf_m) / cf_d;
        seq_t coeff = (seq_t)cf_a;

        /* recurrence q_i = a_i * q_{i-1} + q_{i-2}  for i ≥ 2 */
        seq_t q;
        if (i == 0)          q = (seq_t)1;
        else if (i == 1)     q = (seq_t)1;
        else                 q = coeff * denoms[i-1] + denoms[i-2];

        denoms[i] = q;
    }

    denoms_len = (size_t)n + 1;  /* update length to reflect stored indices */
    return denoms[n];
}
