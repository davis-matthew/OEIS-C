/*
 * A042069  Denominators of continued fraction convergents to sqrt(558).
 * https://oeis.org/A042069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the denominators of the continued‑fraction convergents to sqrt(558).
    These values satisfy the recurrence
        a(n) = 15874*a(n-12) – a(n-24)   for n ≥ 24,
    with the first 24 terms given implicitly by the continued fraction of sqrt(558). */

#include <stdlib.h>
#include <stdint.h>

/* Local state for incremental generation of terms */
static seq_t *cache   = NULL;   /* array of computed denominators          */
static size_t cache_len = 0;    /* allocated size of the cache array       */
static idx_t  max_comp  = 0;    /* number of terms actually computed (0..max_comp-1) */

static unsigned long long cur_a = 0;  /* current partial quotient a_k             */
static unsigned long long cur_m = 0;  /* current m_k                               */
static unsigned long long cur_d = 0;  /* current d_k                               */
static seq_t         prev_q;          /* q_{k-1}                                  */
static seq_t         cur_q;           /* q_k                                      */
static idx_t         cur_k;            /* index of the last computed denominator    */

#define D 558ULL          /* the radicand of sqrt(D)                    */
#define A0 23ULL          /* floor(sqrt(D))                            */

/* Ensure the cache is large enough and return a pointer to the array */
static seq_t *ensure_cache(size_t needed)
{
    if (needed <= cache_len) return cache;

    /* Grow the cache to at least 'needed' elements; use malloc for the demo */
    seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
    if (!new_cache) abort(); /* out of memory */
    /* copy existing values if any */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    /* free old cache */
    free(cache);
    cache = new_cache;
    cache_len = needed;
    return cache;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* First use: initialise the cache and the state for the continued fraction */
    if (cache_len == 0) {
        /* Allocate just enough for the asked index initially */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache) abort(); /* out of memory */
        cache_len = n + 1;

        /* Fundamental constants of the continued fraction for sqrt(D) */
        cur_a = A0;           /* a0 = floor(sqrt(D))                   */
        cur_m = 0ULL;         /* m0 = 0                                 */
        cur_d = 1ULL;         /* d0 = 1                                 */
        prev_q = 0;           /* q_-1 = 0                               */
        cur_q  = 1;           /* q_0  = 1 (the first denominator)       */
        cur_k  = 0;           /* index of the last computed denominator */

        cache[0] = cur_q;     /* a(0) = 1                               */
        max_comp = 1;
    } else if (n + 1 > cache_len) {
        /* Grow the cache if the requested index is beyond current capacity */
        cache = ensure_cache(n + 1);
    }

    /* If we have already computed up to n, just return the value */
    if (max_comp > n) return cache[n];

    /* Generate terms up to index n */
    for (idx_t i = max_comp; i <= n; ++i) {
        /* Produce the next partial quotient a_{i} */
        unsigned long long m_next = cur_d * cur_a - cur_m;
        unsigned long long d_next = (D - m_next * m_next) / cur_d;
        unsigned long long a_next = (A0 + m_next) / d_next;

        /* Compute the next denominator q_{i} using the convergent recurrence */
        seq_t q_next = (seq_t)a_next * cur_q + prev_q;
        cache[i] = q_next;

        /* Shift state variables for the next iteration */
        prev_q = cur_q;
        cur_q  = q_next;
        cur_m  = m_next;
        cur_d  = d_next;
        cur_a  = a_next;
        cur_k  = i;
    }

    max_comp = n + 1;
    return cache[n];
}
