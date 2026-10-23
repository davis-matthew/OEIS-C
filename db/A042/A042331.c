/*
 * A042331  Denominators of continued fraction convergents to sqrt(692).
 * https://oeis.org/A042331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 4999698*a(n-14) - a(n-28) for n > 27 Initial values are
 *            the denominators of the convergents of the continued fraction of
 *            sqrt(692). The first few are: a(0) = 1, a(1) = 3, a(2) = 10,
 *            a(3) = 13, a(4) = 36, a(5) = 49, a(6) = 85, a(7) = 1069, a(8) =
 *            1154, a(9) = 2223, a(10) = 5600, a(11) = 782...
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following function returns the n‑th term of this sequence.  It
   memoises intermediate values so that any order of calls works. */

#include <stdlib.h>
#include <stdint.h>

/* Integer square‑root (floor).  Only used once for a0 = floor(sqrt(692)). */
static idx_t _isqrt_u64(idx_t n) {
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid) low = mid;
        else high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* array of terms */
    static size_t cache_len = 0;     /* number of terms actually stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Expand cache if necessary */
    if (n >= cache_cap) {
        size_t newcap = cache_cap ? cache_cap << 1 : 32;
        while (newcap <= (size_t)n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }

    /* First call: initialise the first 28 terms by computing convergents. */
    if (cache_len == 0) {
        const idx_t D = 692;
        idx_t a0 = _isqrt_u64(D);           /* floor(sqrt(692)) = 26 */

        idx_t m = 0, d = 1, a = a0;
        seq_t p_im2 = 0, p_im1 = 1;         /* p_{-2}, p_{-1} */
        seq_t q_im2 = 1, q_im1 = 0;         /* q_{-2}, q_{-1} */

        for (idx_t k = 0; k < 28; ++k) {
            seq_t p_k = (seq_t)a * p_im1 + p_im2;
            seq_t q_k = (seq_t)a * q_im1 + q_im2;
            cache[k] = q_k;                /* store denominator */

            /* next partial quotient */
            idx_t m_next = d * a - m;
            idx_t d_next = (D - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;

            m = m_next;
            d = d_next;
            a = a_next;

            p_im2 = p_im1;
            p_im1 = p_k;
            q_im2 = q_im1;
            q_im1 = q_k;
        }
        cache_len = 28;
    }

    /* Compute further terms on demand */
    if (n >= cache_len) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i < 28) {
                /* already initialised */
                continue;
            }
            cache[i] = ((seq_t)4999698 * cache[i - 14]) - cache[i - 28];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
