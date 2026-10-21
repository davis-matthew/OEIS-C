/*
 * A209387  a(n) = 2*A005132(n) + 1.
 * https://oeis.org/A209387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A005132(n) + 1; * Sequence A209387: odd‑numbered
 *            Recamán. * a(0)=1; * For n>0: step = 2*n; * if a(n-1) >= step
 *            and (a(n-1)-step) has not appeared before * a(n) = a(n-1) -
 *            step; * else * a(n) = a(n-1) + step;
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;

    /* Initialise storage on first call */
    if (seq == NULL) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return (seq_t)0;   /* allocation failed */
        seq[0] = (seq_t)1;
        len  = 1;
    }

    /* n already computed? */
    if (n < (idx_t)len)
        return seq[(size_t)n];

    /* compute terms up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        seq_t step = (seq_t)i * 2;          /* step = 2*i  (i starts at 1) */
        seq_t cand = 0;
        bool try_sub = false;

        /* try subtraction first if possible */
        if (seq[i - 1] >= step) {
            cand = seq[i - 1] - step;
            try_sub = true;
        }

        /* check if candidate is already used */
        bool seen = false;
        if (try_sub) {
            for (size_t j = 0; j < len; ++j) {
                if (seq[j] == cand) {
                    seen = true;
                    break;
                }
            }
        }

        seq_t newval;
        if (try_sub && !seen)
            newval = cand;   /* use subtraction */
        else
            newval = seq[i - 1] + step;   /* use addition */

        /* ensure capacity for the new element */
        if (len == cap) {
            size_t newcap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
            if (!tmp) { free(seq); return (seq_t)0; } /* allocation failed */
            seq = tmp;
            cap = newcap;
        }

        seq[len++] = newval;
    }

    return seq[(size_t)n];
}
