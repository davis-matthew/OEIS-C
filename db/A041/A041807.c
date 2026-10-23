/*
 * A041807  Denominators of continued fraction convergents to sqrt(424).
 * https://oeis.org/A041807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 64160102 * a(n-18) - a(n-36) for n>35 with base values
 *            a(0)..a(35) computed from the continued‑fraction convergents of
 *            sqrt(424)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;         /* array of computed terms */
static idx_t   cached_nmax = 0;     /* largest index currently stored */

static void init_base_terms(void)
{
    /* compute the first 36 terms (0..35) via the
       continued‑fraction algorithm for sqrt(424)   */
    const unsigned long long D = 424;
    const unsigned long long a0 = 20; /* floor(sqrt(424)) */
    unsigned long long m = 0, d = 1, a = a0;
    seq_t    prev_qminus2 = 1; /* q_{-2} */
    seq_t    prev_qminus1 = 0; /* q_{-1} */
    unsigned i;

    /* allocate array for 36 terms */
    cache = (seq_t *)malloc(36 * sizeof(seq_t));
    cached_nmax = 35;

    for (i = 0; i <= 35; ++i)
    {
        /* compute q_i */
        seq_t q = (seq_t)a * prev_qminus1 + prev_qminus2;
        cache[i] = q;

        /* shift for next iteration */
        prev_qminus2 = prev_qminus1;
        prev_qminus1 = q;

        if (i == 35) break;            /* no more terms needed */

        /* next m, d, a for continued fraction */
        unsigned long long m_next = d * a - m;
        unsigned long long d_next = (D - m_next * m_next) / d;
        unsigned long long a_next = (a0 + m_next) / d_next;
        m = m_next;
        d = d_next;
        a = a_next;
    }
}

seq_t a(idx_t n)
{
    /* initialise the cache on first call */
    if (!cache)
        init_base_terms();

    /* grow the cache if n is beyond the current maximum */
    if (n > cached_nmax)
    {
        /* reallocate larger array */
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));

        /* compute missing terms */
        for (idx_t i = cached_nmax + 1; i <= n; ++i)
        {
            if (i <= 35)
                continue; /* already computed in base terms */

            /* recurrence: a(i) = 64160102 * a(i-18) - a(i-36) */
            seq_t val = cache[i - 18];
            val *= 64160102ull;
            val -= cache[i - 36];
            cache[i] = val;
        }
        cached_nmax = n;
    }

    return cache[n];
}
