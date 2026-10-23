/*
 * A042157  Denominators of continued fraction convergents to sqrt(603).
 * https://oeis.org/A042157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* a(n) = 97684*a(n-10) - a(n-20)   for n>19
   initial values are computed from the continued‑fraction of √603:
   a(0)=1, a(1)=1, a(2)=2, a(3)=7, a(4)=9, a(5)=214,
   a(6)=223, a(7)=883, a(8)=1106, a(9)=1989,
   a(10)=96578, a(11)=98567, a(12)=195145, a(13)=684002,
   a(14)=879147, a(15)=20904383, a(16)=21783530,
   a(17)=86254973, a(18)=108038503, a(19)=194293476 */

seq_t a(idx_t n)
{
    /* ---------- Precompute the first 20 terms from the CF of √603 ---------- */
    static seq_t init[20];
    static int init_done = 0;
    if (!init_done) {
        const uint64_t N      = 603;
        const uint64_t a0     = 24;          /* floor(√603) */
        uint64_t      m       = 0;
        uint64_t      d       = 1;
        uint64_t      a       = a0;

        /* q(-1) = 0, q0 = 1 */
        seq_t q_prev2 = 0;   /* q(-1) */
        seq_t q_prev1 = 1;   /* q0 */
        init[0] = q_prev1;   /* a(0) */

        for (size_t i = 1; i < 20; ++i) {
            /* next partial quotient */
            m = d * a - m;
            d = (N - m * m) / d;
            a = (a0 + m) / d;

            /* q_i = a * q_{i-1} + q_{i-2} */
            seq_t q_curr = ((seq_t)a) * q_prev1 + q_prev2;
            init[i] = q_curr;

            q_prev2 = q_prev1;
            q_prev1 = q_curr;
        }
        init_done = 1;
    }

    if ((idx_t)n < 20) {
        return init[(size_t)n];
    }

    /* ---------- Memoised recurrence for n >= 20 ---------- */
    static seq_t *cache   = NULL;
    static idx_t  cache_sz = 0;

    if (cache == NULL) {
        cache_sz = 20;
        cache    = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (cache == NULL) {
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < 20; ++i) {
            cache[i] = init[i];
        }
    }

    /* Extend cache if necessary */
    if ((idx_t)n >= cache_sz) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        for (idx_t i = cache_sz; i <= (idx_t)n; ++i) {
            cache[i] = ((seq_t)97684) * cache[i - 10] - cache[i - 20];
        }
        cache_sz = new_size;
    }

    return cache[(size_t)n];
}
