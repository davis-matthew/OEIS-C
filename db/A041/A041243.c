/*
 * A041243  Denominators of continued fraction convergents to sqrt(133).
 * https://oeis.org/A041243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued–fraction convergents to sqrt(133) */
#include <stdlib.h>

#if defined(__cplusplus)
# include <cstddef>
#endif

/* floor(sqrt(133)) */
static const unsigned a0 = 11;

/* Period of the continued fraction.  The period ends when a_n = 2*a0. */
static unsigned *period     = NULL;
static size_t   period_len  = 0;

static void init_period(void)
{
    if (period != NULL) return;           /* already initialised */

    size_t cap = 32;
    unsigned *buf = (unsigned *)malloc(cap * sizeof(unsigned));
    size_t   len = 0;

    uint64_t m       = 0;
    uint64_t d       = 1;
    uint64_t a_prev  = a0;                /* a_{n-1} */

    for (;;)
    {
        uint64_t m_next = d * a_prev - m;
        uint64_t d_next = (133 - m_next * m_next) / d;
        unsigned a_next = (unsigned)((a0 + m_next) / d_next);

        if (len == cap)
        {
            cap <<= 1;
            buf = (unsigned *)realloc(buf, cap * sizeof(unsigned));
        }
        buf[len++] = a_next;

        if (a_next == 2 * a0) break;      /* end of period */

        a_prev = a_next;
        m      = m_next;
        d      = d_next;
    }

    period    = buf;
    period_len = len;
}

/* Cache of denominators q_k.  q_{-1}=0, q_0=1.  */
static seq_t *q_cache    = NULL;
static size_t q_cache_cap = 0;
static idx_t  q_cache_max = 0;            /* highest index computed */

static void init_q_cache(void)
{
    if (q_cache != NULL) return;

    size_t cap = 2;
#ifdef __cplusplus
    q_cache = new seq_t[cap];
#else
    q_cache = (seq_t *)malloc(cap * sizeof(seq_t));
#endif
    q_cache[0] = (seq_t)1;                 /* q_0 = 1 */
    q_cache_cap  = cap;
    q_cache_max  = 0;
}

static void ensure_q_up_to(idx_t n)
{
    init_period();
    init_q_cache();

    if (q_cache_max >= n) return;

    for (idx_t k = q_cache_max + 1; k <= n; ++k)
    {
        unsigned a_k = period[(k - 1) % period_len];   /* a_k for k≥1 */
        seq_t prev1  = q_cache[k - 1];
        seq_t prev2  = (k >= 2) ? q_cache[k - 2] : (seq_t)0;
        seq_t new_q  = (seq_t)a_k * prev1 + prev2;

        if ((size_t)(k + 1) > q_cache_cap)
        {
#ifdef __cplusplus
            size_t new_cap = q_cache_cap ? q_cache_cap * 2 : 2;
            while ((size_t)(k + 1) > new_cap) new_cap <<= 1;
            seq_t *tmp = new seq_t[new_cap];
            for (size_t i = 0; i <= (size_t)q_cache_max; ++i)
                tmp[i] = q_cache[i];
            delete [] q_cache;
            q_cache = tmp;
            q_cache_cap = new_cap;
#else
            size_t new_cap = q_cache_cap ? q_cache_cap * 2 : 2;
            while ((size_t)(k + 1) > new_cap) new_cap <<= 1;
            q_cache = (seq_t *)realloc(q_cache, new_cap * sizeof(seq_t));
            q_cache_cap = new_cap;
#endif
        }
        q_cache[k] = new_q;
        q_cache_max = k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;           /* first term */
    ensure_q_up_to(n);
    return q_cache[n];
}
