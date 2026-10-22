/*
 * A005289  Number of graphs on n nodes with 3 cliques.
 * https://oeis.org/A005289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: x^3*(1+x+x^2+3*x^3) / ((1+x+x^2)*(1+x)^2*(x-1)^6), i.e.
 *            a(n) = 3*a(n-1) - a(n-2) - 4*a(n-3) + 2*a(n-4) + 2*a(n-5) +
 *            2*a(n-6) - 4*a(n-7) - a(n-8) + 3*a(n-9) - a(n-10) + [numerator
 *            terms], a(0)=a(1)=a(2)=0, a(3)=1, a(4)=4, a(5)=12,
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    /* denominator coefficients Q[0..10] */
    static const int Q[] = {1, -3, 1, 4, -2, -2, -2, 4, 1, -3, 1};
    /* numerator coefficients P[0..6] */
    static const int P[] = {0, 0, 0, 1, 1, 1, 3};

    static seq_t *vals = NULL;
    static idx_t capacity = 0;
    static idx_t computed = 0;

    if (vals == NULL) {
        capacity = 128;
        vals = alloc_zero(capacity);
        computed = 0; /* vals[0] = 0 */
    }

    if (n >= capacity) {
        idx_t newcap = n + 128;
        vals = grow_zero(vals, capacity, newcap);
        capacity = newcap;
    }

    while (computed < n) {
        idx_t m = computed + 1;
        seq_t pos = 0, neg = 0;
        int j;

        if (m < 7)
            pos += (seq_t)P[m];

        for (j = 1; j <= 10 && (idx_t)j <= m; j++) {
            int q = Q[j];
            if (q < 0)
                pos += (seq_t)(-q) * vals[m - j];
            else
                neg += (seq_t)q * vals[m - j];
        }

        vals[m] = pos - neg;
        computed = m;
    }

    return vals[n];
}
