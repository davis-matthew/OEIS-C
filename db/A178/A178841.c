/*
 * A178841  The number of pure inverting compositions of n.
 * https://oeis.org/A178841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n ≥ 1, let p(m) be the partition numbers. Then
 *            a(n) = 2^(n-1) - Σ_{k=0}^{n-1} a(k) * p(n-k). The sequence a(n)
 *            counts pure inverting compositions of n
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    static seq_t *a_arr = NULL;
    static seq_t *p_arr = NULL;
    static size_t cap   = 0;          /* array capacity */
    static size_t last  = 0;          /* maximum index computed */

    /* initial allocation */
    if (!a_arr) {
        size_t init = 8;
        a_arr = (seq_t*)malloc(init * sizeof(seq_t));
        p_arr = (seq_t*)malloc(init * sizeof(seq_t));
        cap   = init;
    }

    /* grow arrays if necessary */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        a_arr = (seq_t*)realloc(a_arr, newcap * sizeof(seq_t));
        p_arr = (seq_t*)realloc(p_arr, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* compute missing terms up to n */
    for (size_t i = last; i <= (size_t)n; ++i) {
        if (i == 0) {
            a_arr[0] = 1;
            p_arr[0] = 1;
            continue;
        }

        /* compute partition number p[i] using the pentagonal‑number recurrence */
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        for (int k = 1; ; ++k) {
            idx_t pent1 = (idx_t)k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            int sign = (k & 1) ? 1 : -1;
            if (sign > 0) pos_sum += p_arr[i - pent1];
            else          neg_sum += p_arr[i - pent1];

            idx_t pent2 = (idx_t)k * (3 * k + 1) / 2;
            if (pent2 > i) continue;
            if (sign > 0) pos_sum += p_arr[i - pent2];
            else          neg_sum += p_arr[i - pent2];
        }
        p_arr[i] = pos_sum - neg_sum;   /* pos_sum ≥ neg_sum holds */

        /* compute a[i] via convolution with p */
        seq_t sum = 0;
        for (size_t k = 0; k < i; ++k) {
            sum += a_arr[k] * p_arr[i - k];
        }
        seq_t two_pow = pow2((idx_t)i - 1);
        a_arr[i] = two_pow - sum;       /* sum ≤ two_pow */
    }
    last = (size_t)n;
    return a_arr[n];
}
