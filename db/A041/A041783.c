/*
 * A041783  Denominators of continued fraction convergents to sqrt(412).
 * https://oeis.org/A041783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 18, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents to sqrt(412)
 * a(0)=1, a(1)=3, a(2)=7, a(3)=10, a(4)=37, a(5)=47, a(6)=225, a(7)=722,
 * a(8)=1669, a(9)=22419, a(10)=225859, a(11)=2958586, a(12)=6143031,
 * a(13)=21387679, a(14)=91693747, a(15)=113081426, a(16)=430938025,
 * a(17)=544019451, a(18)=1518976927, a(19)=5100950232, a(20)=205556986207,
 * a(21)=621771908853, a(22)=1449100803913, a(23)=2070872712766;
 * For n>39 : a(n)=207075963134*a(n-20) - a(n-40)  (recurrence from the period). */

#include <stdlib.h>
#include <stdint.h>

static const idx_t period_coeff[20] = {
    3, 2, 1, 3, 1, 4, 3, 2, 13, 10,
    13, 2, 3, 4, 1, 3, 1, 2, 3, 40
};

seq_t a(idx_t n)
{
    /* static cache of computed denominators */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of initialized elements */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* constant in the recurrence */
    const seq_t C = (seq_t)207075963134ULL;

    /* ---- lazy initialisation of first 40 terms via continued fraction ---- */
    if (cache_size == 0) {
        cache_cap  = 40;
        cache_size = 40;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* out‑of‑memory, but unlikely in tests */

        /* compute denominators q_i for i=0..39 */
        seq_t q_prev2 = (seq_t)1;   /* q_{-2} */
        seq_t q_prev1 = (seq_t)0;   /* q_{-1} */

        for (idx_t i = 0; i < 40; ++i) {
            seq_t a_coeff;
            if (i == 0) {              /* a_0 = floor(sqrt(412)) = 20 */
                a_coeff = (seq_t)20;
            } else {
                a_coeff = (seq_t)period_coeff[(i - 1) % 20];
            }
            seq_t q_i = a_coeff * q_prev1 + q_prev2;
            cache[i] = q_i;
            q_prev2 = q_prev1;
            q_prev1 = q_i;
        }
    }

    /* ---- extend cache if necessary ---- */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence: a(i) = C*a(i-20) - a(i-40) */
            seq_t val = C * cache[i - 20] - cache[i - 40];

            /* enlarge array if needed */
            if (i >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap <= i) new_cap = i + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* out‑of‑memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
