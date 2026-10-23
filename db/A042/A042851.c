/*
 * A042851  Denominators of continued fraction convergents to sqrt(956).
 * https://oeis.org/A042851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominators of continued‑fraction convergents to
 *            sqrt(956). Computation is based on the standard recurrence
 *            p_{k+1} = a_{k+1} p_k + p_{k-1} q_{k+1} = a_{k+1} q_k + q_{k-1}
 *            with initial values p_{-1}=1, p_0=30, q_{-1}=0, q_0=1. The
 *            partial quotients a_k are produced by the classic continue...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const idx_t RADICAND = 956;          /* N = 956              */
static const int  ROOT0     = 30;           /* floor(sqrt(956))     */

static seq_t *cache   = NULL;               /* cache of q_k          */
static size_t cap     = 0;                  /* capacity of cache     */
static idx_t last    = 0;                   /* highest index cached  */

static seq_t p_prevprev, p_prev;            /* p_{k-1} and p_{k}     */
static seq_t q_prevprev, q_prev;            /* q_{k-1} and q_{k}     */
static idx_t m, d;                          /* continued‑fraction state */
static int   a_cur;                         /* current partial quotient a_k */

/* Ensure the cache can hold at least n+1 entries. */
static void grow_cache(idx_t n)
{
    if (n < (idx_t)cap) return;
    size_t new_cap = cap ? cap << 1 : 64;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { cap = 0; cache = NULL; return; }  /* out of memory */
    cache = tmp;
    cap   = new_cap;
}

/* Initialise state for the first term (k = 0). */
static void init_state(void)
{
    p_prevprev = (seq_t)1;           /* p_{-1} */
    p_prev    = (seq_t)ROOT0;        /* p_0   */
    q_prevprev = (seq_t)0;           /* q_{-1} */
    q_prev    = (seq_t)1;            /* q_0   */
    m = 0;                          /* m_0   */
    d = 1;                          /* d_0   */
    a_cur = ROOT0;                  /* a_0   */
    cache[0] = q_prev;              /* a(0) = q_0 = 1  */
    last = 0;
}

seq_t a(idx_t n)
{
    /* First call: initialise cache and state. */
    if (!cache) {
        grow_cache(n);
        if (!cache) return (seq_t)0; /* allocation failed */
        init_state();
    }

    /* If the requested index is already computed, return it. */
    if (n <= last) return cache[n];

    /* Ensure the cache holds the requested index. */
    grow_cache(n);
    if (!cache) return (seq_t)0;     /* allocation failed */

    /* Compute terms from last+1 up to n. */
    for (idx_t k = last + 1; k <= n; ++k) {
        /* Compute next partial quotient a_{k+1}. */
        idx_t m_next = d * (idx_t)a_cur - m;
        idx_t d_next = (idx_t)((RADICAND - m_next * m_next) / d);
        int   a_next = (int)((ROOT0 + m_next) / d_next);

        /* New convergents. */
        seq_t p_new = (seq_t)a_next * p_prev + p_prevprev;
        seq_t q_new = (seq_t)a_next * q_prev + q_prevprev;

        cache[k] = q_new;

        /* Shift state for next iteration. */
        p_prevprev = p_prev;
        p_prev = p_new;
        q_prevprev = q_prev;
        q_prev = q_new;
        m = m_next;
        d = d_next;
        a_cur = a_next;
    }
    last = n;
    return cache[n];
}
