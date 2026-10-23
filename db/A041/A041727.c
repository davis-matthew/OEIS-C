/*
 * A041727  Denominators of continued fraction convergents to sqrt(383).
 * https://oeis.org/A041727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 37536*a(n-8) - a(n-16) for n > 15. Initial terms (0‑15):
 *            1, 1, 2, 7, 135, 412, 547, 959, 36989, 37948, 74937, 262759,
 *            5067358, 15464833, 20532191, 35997024. These first sixteen
 *            numbers are generated automatically from the continued‑fraction
 *            expansion of √383
 * Range:     u8 none, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Generate first 16 terms lazily on first call. */
    if (cache_len == 0) {
        const idx_t init_len = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * init_len);
        if (!cache) return (seq_t)0;   /* allocation failed */

        /* Continued‑fraction data for √383 */
        seq_t m = (seq_t)0;            /* m₀ */
        seq_t d = (seq_t)1;            /* d₀ */
        seq_t a0 = (seq_t)19;          /* a₀ = ⌊√383⌋ */

        seq_t q_prev2 = (seq_t)1;      /* q_{-2} */
        seq_t q_prev1 = (seq_t)0;      /* q_{-1} */

        for (idx_t i = 0; i < init_len; ++i) {
            /* q_i = a_i * q_{i-1} + q_{i-2} */
            seq_t q = a0 * q_prev1 + q_prev2;
            cache[i] = q;

            /* advance continued fraction:
               m_{k+1} = d_k * a_k – m_k
               d_{k+1} = (383 – m_{k+1}²) / d_k
               a_{k+1} = ⌊(19 + m_{k+1}) / d_{k+1}⌋ */
            seq_t m_next = d * a0 - m;
            seq_t d_next = ((seq_t)383 - m_next * m_next) / d;
            seq_t a_next = ((seq_t)19 + m_next) / d_next;

            m = m_next;
            d = d_next;
            a0 = a_next;

            /* shift the previous q values */
            q_prev2 = q_prev1;
            q_prev1 = q;
        }

        cache_len = init_len;
    }

    /* Return cached value if already available */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache up to n (inclusive) */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp) return (seq_t)0;   /* allocation failed */
    cache = tmp;

    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)37536 * cache[i - 8] - cache[i - 16];
    }
    cache_len = new_len;

    return cache[n];
}
