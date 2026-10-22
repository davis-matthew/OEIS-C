/*
 * A003727  Expansion of e.g.f. exp(x * cosh(x)).
 * https://oeis.org/A003727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} binomial(n-1,2*k) * (2*k+1) *
 *            a(n-2*k-1), a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    static seq_t *A = NULL;   /* computed terms a(0..done-1) */
    static seq_t *C = NULL;   /* Pascal row (done-1): C[j] = binomial(done-1, j) */
    static idx_t cap = 0;
    static idx_t done = 0;

    if (n >= cap) {
        idx_t newcap = cap ? cap * 2 : 16;
        if (newcap < n + 1) newcap = n + 1;
        A = grow_zero(A, cap, newcap);
        C = grow_zero(C, cap, newcap);
        cap = newcap;
    }
    if (done == 0) {
        A[0] = 1;
        C[0] = 1;
        done = 1;
    }

    while (done <= n) {
        idx_t i = done;
        seq_t sum = 0;
        idx_t k;
        for (k = 0; 2 * k <= i - 1; k++) {
            sum += C[2 * k] * (seq_t)(2 * k + 1) * A[i - 2 * k - 1];
        }
        A[i] = sum;
        /* advance Pascal row from i-1 to i */
        {
            idx_t j;
            for (j = i; j >= 1; j--) {
                C[j] += C[j - 1];
            }
        }
        done++;
    }

    return A[n];
}
