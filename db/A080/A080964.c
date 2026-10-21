/*
 * A080964  Euler transform of period-16 sequence
 *          [2,-3,2,1,2,-3,2,-6,2,-3,2,1,2,-3,2,-3,...].
 * https://oeis.org/A080964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n≥1, a(n) = (1/n) * Σ_{k=1..n} (Σ_{d|k} d * c_d) *
 *            a(n-k), where c_d = 2,-3,2,1,2,-3,2,-6,2,-3,2,1,2,-3,2,-3
 *            repeating
 * Range:     i8 n <= 19, i16 n <= 734, i32 n <= 9345, i64 n <= 9345,
 *            i128 n <= 9345, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Periodic sequence c_d */
static const seq_t period[16] = {
    2, -3, 2, 1,
    2, -3, 2, -6,
    2, -3, 2, 1,
    2, -3, 2, -3
};

/* Helper to get c_d for d≥1 */
static inline seq_t get_c(idx_t d) {
    return period[(d - 1) & 15]; /* 16 = 2^4 */
}

/* Euler transform of c_d via recurrence */
seq_t a(idx_t n) {
    /* static caches */
    static seq_t *coef = NULL;        /* a(n) */
    static seq_t *s_val = NULL;       /* S(k) = Σ_{d|k} d*c_d */
    static size_t cap = 0;            /* allocated capacity */
    static size_t size = 0;           /* highest computed index + 1 */

    /* Ensure capacity */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = cap > 0 ? cap : 16;
        while (new_cap < need) new_cap <<= 1;
        coef = (seq_t *)realloc(coef, new_cap * sizeof(seq_t));
        s_val = (seq_t *)realloc(s_val, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing S(k) values if any */
    if (size <= n) {
        for (idx_t k = (idx_t)size; k <= n; ++k) {
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    seq_t cd = get_c(d);
                    sum += (seq_t)d * cd;
                    idx_t d2 = k / d;
                    if (d2 != d) {
                        seq_t cd2 = get_c(d2);
                        sum += (seq_t)d2 * cd2;
                    }
                }
            }
            s_val[k] = sum;
        }
    }

    /* Compute a[n] values sequentially */
    if (size == 0) {
        coef[0] = 1;          /* a(0) = 1 */
        size = 1;
    }
    for (idx_t m = (idx_t)size; m <= n; ++m) {
        if (m == 0) {
            coef[0] = 1;
        } else {
            seq_t acc = 0;
            for (idx_t k = 1; k <= m; ++k) {
                acc += s_val[k] * coef[m - k];
            }
            coef[m] = acc / (seq_t)m;   /* exact division */
        }
    }
    size = (size_t)n + 1;

    return coef[n];
}
