/*
 * A170226  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence satisfies a linear recurrence of order 40. For 0 ≤
 *            n ≤ 39: a(n) = coeff_N(n) + 23*∑_{i=0}^{n-1} a(i) where
 *            coeff_N(0)=1, coeff_N(n)=2 for 1≤n≤39. For n = 40: a(40) =
 *            23*∑_{i=1}^{39} a(i) - 276*a(0) + 1 For n ≥ 41: a(n) =
 *            23*∑_{i=n-39}^{n-1} a(i) - 276*a(n-40) This recurrence comes
 *            fr...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed terms */
    static seq_t *buf = NULL;
    static size_t buf_capacity = 0;
    static idx_t up_to = 0;      /* number of terms already computed */

    /* Helper: current index to compute (as size_t for array access) */
    size_t idx;

    /* Allocate initial buffer if needed */
    if (buf == NULL) {
        buf_capacity = 64;
        buf = (seq_t *)malloc(buf_capacity * sizeof(seq_t));
        if (buf == NULL) return (seq_t)0; /* out‑of‑memory guard */
    }

    /* If the requested n is already computed, just return it */
    if (n < up_to) return buf[(size_t)n];

    /* Compute terms progressively up to n */
    for (idx = up_to; idx <= (size_t)n; ++idx) {
        seq_t val;
        seq_t sum;
        /* Ensure we have enough capacity */
        if (idx >= buf_capacity) {
            size_t new_cap = buf_capacity * 2;
            if (new_cap <= idx) new_cap = idx + 1;
            seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0; /* out‑of‑memory guard */
            buf = tmp;
            buf_capacity = new_cap;
        }

        if (idx <= 39) {                   /* 0 … 39 */
            seq_t coeff = (idx == 0) ? (seq_t)1 : (seq_t)2;
            sum = (seq_t)0;
            for (size_t j = 0; j < idx; ++j) sum += buf[j];   /* ∑ a(i) i<idx */
            val = coeff + (seq_t)23 * sum;
        } else if (idx == 40) {            /* 40 */
            sum = (seq_t)0;
            for (size_t j = 1; j <= 39; ++j) sum += buf[j];   /* ∑ a(i) i=1…39 */
            val = (seq_t)23 * sum - (seq_t)276 * buf[0] + (seq_t)1;
        } else {                           /* idx ≥ 41 */
            sum = (seq_t)0;
            for (size_t j = idx - 39; j < idx; ++j) sum += buf[j]; /* last 39 */
            val = (seq_t)23 * sum - (seq_t)276 * buf[idx - 40];
        }

        buf[idx] = val;
    }

    up_to = n + 1;          /* all terms up to n are now ready */
    return buf[(size_t)n];
}
