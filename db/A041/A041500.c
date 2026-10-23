/*
 * A041500  Numerators of continued fraction convergents to sqrt(267).
 * https://oeis.org/A041500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent of √267. Recurrence:
 *            p_{-1} = 1, p_0 = 16, p_k = a_k · p_{k-1} + p_{k-2} (k ≥ 1),
 *            where a_k are the partial quotients of the continued fraction of
 *            √267
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 29, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* --- static storage for cached terms and CF state -------------------------------- */

static seq_t *cache      = NULL;   /* cached numerators a(0), a(1), … */
static size_t cache_cap  = 0;       /* current capacity of cache   */
static size_t cache_len  = 0;       /* number of terms actually stored */

static idx_t  computed_max = (idx_t)-1;  /* largest n that has been produced */

/* continued fraction state for √267 ------------------------------------------- */

static const uint64_t D   = 267;  /* D in √D  */
static const uint64_t A0  = 16;   /* floor(√267) */

static uint64_t m_state = 0;      /* m_k   */
static uint64_t d_state = 1;      /* d_k   */
static uint64_t a_state = 16;     /* a_k   */

/* --- helper to (re)allocate the cache -------------------------------------- */

static void grow_cache(idx_t need)
{
    if ((size_t)need < cache_cap) return;
    while (cache_cap <= (size_t)need) cache_cap <<= 1;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!cache) abort();   /* unrecoverable in a contest environment */
}

/* --- initialisation of static data ------------------------------------------ */

static void init_sequence(void)
{
    if (cache) return;                 /* already initialised */

    cache_cap = 16;
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (!cache) abort();

    cache[0] = (seq_t)A0;              /* a(0) = 16 */
    cache_len = 1;
    computed_max = 0;

    /* keep the recurrence state for numerators (p_{-1}, p_0) */
    static seq_t p_prev2 = (seq_t)1;   /* p_{-1} */
    static seq_t p_prev1 = (seq_t)A0;  /* p_0   */
    (void)p_prev2; (void)p_prev1;      /* silence unused‑static‑variable warnings */
}

/* ------------------------------------------------------------------------- */
/*  a(idx_t n) :  returns the n-th term of the sequence                      */
/* ------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    init_sequence();                     /* ensure cache and state exist */

    /* If we have already computed up to > n, just return it. */
    if (cache_len > n) return cache[n];

    /* Grow the cache up to index n. */
    grow_cache(n);

    /* Keep the recurrence state across successive calls. */
    static seq_t p_prev2 = (seq_t)1;   /* p_{k-2} */
    static seq_t p_prev1 = (seq_t)A0;  /* p_{k-1} */

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* compute next continued‐fraction partial quotient a_i */
        uint64_t m_next = d_state * a_state - m_state;
        uint64_t d_next = (D - m_next * m_next) / d_state;
        uint64_t a_next = (A0 + m_next) / d_next;

        /* recurrence on numerators: p_k = a_k·p_{k-1} + p_{k-2} */
        seq_t p_cur = ((seq_t)a_next) * p_prev1 + p_prev2;
        cache[i] = p_cur;

        /* shift state for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
        m_state = m_next;
        d_state = d_next;
        a_state = a_next;
    }

    computed_max = n;
    cache_len = (size_t)(n + 1);
    return cache[n];
}
