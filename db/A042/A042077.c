/*
 * A042077  Denominators of continued fraction convergents to sqrt(562).
 * https://oeis.org/A042077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 441876994*a(n-18) - a(n-36) for n>35. a(0)=1, a(1)=1,
 *            a(2)=3, a(3)=7, a(4)=17, a(5)=75, a(6)=92, a(7)=535, a(8)=627,
 *            a(9)=14329, a(10)=14956, a(11)=89109, a(12)=104065,
 *            a(13)=505369, a(14)=1114803, a(15)=2734975, a(16)=6584753,
 *            a(17)=9319728, a(18)=435292241, a(19)=444611969,
 *            a(20)=1324516...
 * Range:     u8 none, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 80,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Ensure the first 36 terms are available */
    if (cache_size < 36) {
        size_t new_size = 36;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;

        const seq_t D = 562;
        const seq_t a0 = 23;     /* floor(sqrt(562)) */

        seq_t m = 0;
        seq_t d = 1;
        seq_t a_cur = a0;

        seq_t p_prev2 = 0, p_prev1 = 1;
        seq_t q_prev2 = 1, q_prev1 = 0;

        for (size_t i = 0; i < 36; ++i) {
            seq_t p = a_cur * p_prev1 + p_prev2;
            seq_t q = a_cur * q_prev1 + q_prev2;
            cache[i] = q;            /* denominator of the i‑th convergent */

            /* next partial quotient */
            seq_t m_next = d * a_cur - m;
            seq_t d_next = (D - m_next * m_next) / d;
            seq_t a_next = (a0 + m_next) / d_next;

            m = m_next;
            d = d_next;
            a_cur = a_next;

            p_prev2 = p_prev1;
            p_prev1 = p;
            q_prev2 = q_prev1;
            q_prev1 = q;
        }
        cache_size = new_size;
    }

    if (n < cache_size)
        return cache[n];

    /* Grow cache to accommodate n */
    size_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;

    /* Compute terms beyond the initial 36 using the recurrence */
    for (size_t i = cache_size; i < new_size; ++i) {
        seq_t prod = mul_small(cache[i - 18], 441876994u);
        cache[i] = prod - cache[i - 36];
    }

    cache_size = new_size;
    return cache[n];
}
