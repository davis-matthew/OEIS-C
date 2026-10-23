/*
 * A014278  Number of trees on n nodes with 3 forbidden limbs of size 4, 5 and
 *          6.
 * https://oeis.org/A014278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of (x - x^4 - x^5 - x^6 + 2*x^7) * ∏_{p≥1} (1
 *            - x^p)^(-a(p)) (implicit definition)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Public interface -------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static storage for memoization */
    static idx_t max_n = 0;
    static seq_t *at = NULL;     /* a[0..max_n]   */
    static seq_t *bt = NULL;     /* b[0..max_n]   */
    static seq_t *sigma = NULL;  /* sigma[0..max_n] */

    /* ---------- Initial allocation ------------------------------------- */
    if (max_n == 0) {
        max_n = 0;
        at     = (seq_t*)malloc(sizeof(seq_t));      /* a[0] */
        bt     = (seq_t*)malloc(sizeof(seq_t));      /* b[0] */
        sigma  = (seq_t*)malloc(sizeof(seq_t));      /* sigma[0] */

        at[0]     = (seq_t)0;
        bt[0]     = (seq_t)1;      /* b(0) = 1  */
        sigma[0]  = (seq_t)0;
    }

    if (n == 0) return at[0];  /* a(0) = 0  */

    /* ---------- Extend arrays if necessary ----------------------------- */
    if (n > max_n) {
        idx_t old_max = max_n;
        idx_t new_max = n;

        /* Reallocate to new size (new_max + 1) */
        at =     (seq_t*)realloc(at,     (size_t)(new_max + 1) * sizeof(seq_t));
        bt =     (seq_t*)realloc(bt,     (size_t)(new_max + 1) * sizeof(seq_t));
        sigma =  (seq_t*)realloc(sigma,  (size_t)(new_max + 1) * sizeof(seq_t));

        /* Initialise new slots */
        for (idx_t i = old_max + 1; i <= new_max; ++i) {
            at[i]    = (seq_t)0;
            bt[i]    = (seq_t)0;
            sigma[i] = (seq_t)0;
        }

        /* ---- Update sigma for multiples of earlier a's
           that lie beyond the old maximum ---- */
        if (old_max > 0) {
            for (idx_t d = 1; d <= old_max; ++d) {
                seq_t ad = at[d];
                if (ad == (seq_t)0) continue;
                for (idx_t m = d; m <= new_max; m += d) {
                    if (m <= old_max) continue;   /* already correct */
                    sigma[m] += (seq_t)d * ad;
                }
            }
        }

        /* ---- Compute a, update sigma, and compute b for new indices ---- */
        for (idx_t i = old_max + 1; i <= new_max; ++i) {
            /* a(i) = b(i-1) - b(i-4) - b(i-5) - b(i-6) + 2*b(i-7) */
            seq_t ai = (seq_t)0;
            if (i >= 1) ai += bt[i - 1];
            if (i >= 4) ai -= bt[i - 4];
            if (i >= 5) ai -= bt[i - 5];
            if (i >= 6) ai -= bt[i - 6];
            if (i >= 7) ai += (seq_t)2 * bt[i - 7];
            at[i] = ai;

            /* Update sigma for multiples of i up to new_max */
            seq_t mult = (seq_t)i * ai;
            for (idx_t m = i; m <= new_max; m += i) {
                sigma[m] += mult;
            }

            /* b(i) = (1/i) * Σ_{k=1}^{i} sigma(k) * b(i-k) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += sigma[k] * bt[i - k];
            }
            bt[i] = sum / (seq_t)i;
        }

        max_n = new_max;
    }

    return at[n];
}
