/*
 * A042674  Numerators of continued fraction convergents to sqrt(867).
 * https://oeis.org/A042674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 140452*a(n-6) - a(n-12) (a(0)=29, a(1)=59, a(2)=265,
 *            a(3)=7744, a(4)=31241, a(5)=70226, a(6)=4104349, a(7)=8278924,
 *            a(8)=37220045, a(9)=1087660229, a(10)=4387860961,
 *            a(11)=9863382151)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* --- initialise first 12 terms from the continued fraction of sqrt(867) --- */
    if (cache_len == 0) {
        const idx_t N  = 867;
        const idx_t a0 = 29;      /* floor(sqrt(867))     */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;

        /* allocate space for at least 12 terms */
        cache_cap = 32;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;

        seq_t h_prev2 = (seq_t)0, h_prev1 = (seq_t)1;

        for (idx_t i = 0; i < 12; ++i) {
            seq_t h = (seq_t)a * h_prev1 + h_prev2;   /* convergent numerator */
            cache[i] = h;

            h_prev2 = h_prev1;
            h_prev1 = h;

            /* next partial quotient */
            idx_t m_next = d * a - m;
            idx_t d_next = (N - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;
            m = m_next; d = d_next; a = a_next;
        }
        cache_len = 12;
    }

    if (n < cache_len)
        return cache[n];

    /* --- ensure enough capacity for index n --- */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 16;
        while ((idx_t)(n + 1) > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* --- compute missing terms via the recurrence --- */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)140452 * cache[i - 6] - cache[i - 12];
    }
    cache_len = n + 1;
    return cache[n];
}
