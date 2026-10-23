/*
 * A042829  Denominators of continued fraction convergents to sqrt(945).
 * https://oeis.org/A042829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A042829: a(n) = denominator of the nth convergent of √945
 * (continued‑fraction denominators).  Generated lazily with memoisation. */
/* Stores only the latest partial quotient and the last two denominators to
 * compute the next term:  q[n] = a[n] * q[n‑1] + q[n‑2]   with
 *  a[0] = ⌊√945⌋ = 30.  */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

static const idx_t D = 945ULL;          /* the squarefree integer */
static const idx_t SQRT_D = 30ULL;      /* ⌊√945⌋ */

/* Cache for already computed denominators */
static seq_t  *cache      = NULL;
static size_t  cache_cap  = 0;
static size_t  cache_len  = 0;

/* State of the continued fraction */
static idx_t   a_curr = SQRT_D;   /* current partial quotient a[n]   */
static idx_t   m      = 0;        /* m[n] in the recurrence         */
static idx_t   d      = 1;        /* d[n] in the recurrence         */
static seq_t   q_prevprev = 1;    /* q[n‑2] (initially q[–2] = 1)   */
static seq_t   q_prev    = 0;     /* q[n‑1] (initially q[–1] = 0)   */
static bool    initialized = false;

/* Append a new term to the cache, growing it if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!ptr) {
            /* allocation failed – terminate */
            exit(1);
        }
        cache = ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Initialise the cache and the CF state on the first call. */
static void init(void)
{
    /* Allocate initial buffer */
    cache_cap = 8;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);

    /* Generate q[0] */
    seq_t q_next = (seq_t)a_curr * q_prev + q_prevprev;   /* = 1 */
    cache_append(q_next);                                 /* n=0 */

    /* Shift denominator history */
    q_prevprev = q_prev;  /* 0 */
    q_prev    = q_next;   /* 1 */

    /* Compute a[1] for the next iteration */
    idx_t m_next = d * a_curr - m;
    idx_t d_next = (D - m_next * m_next) / d;
    idx_t a_next = (SQRT_D + m_next) / d_next;

    m = m_next;
    d = d_next;
    a_curr = a_next;

    initialized = true;
}

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (!initialized) {
        init();
        /* If the requested index is 0 we already have it */
        if (n == 0) return cache[0];
    }

    /* Generate terms up to the requested index. */
    while (cache_len <= n) {
        /* Compute next denominator q[n] */
        seq_t q_next = (seq_t)a_curr * q_prev + q_prevprev;
        cache_append(q_next);

        /* Shift denominator history for next step */
        q_prevprev = q_prev;
        q_prev    = q_next;

        /* Compute next partial quotient a[n+1] */
        idx_t m_next = d * a_curr - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (SQRT_D + m_next) / d_next;

        m = m_next;
        d = d_next;
        a_curr = a_next;
    }

    return cache[n];
}
