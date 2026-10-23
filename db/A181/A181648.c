/*
 * A181648  Expansion of x^(-2/3) * psi(x) * c(x^2) / 3 in powers of x where
 *          psi() is a Ramanujan theta function and c() is a cubic AGM theta
 *          function.
 * https://oeis.org/A181648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   3*a(n) = A008443(3*n+2); equivalently a(n) is the Euler
 *            transform of the period‑6 sequence [1,0,1,0,1,-3]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   period6_b
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/period6_b.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static buffers to memoise computed values */
    static seq_t *a_val = NULL;        /* a(n) values */
    static long long *c_val = NULL;    /* c(k) = Σ d|k d * b(d)  (signed) */
    static idx_t cap   = 0;            /* current maximum computed index */

    /* initialise base case a(0)=1 */
    if (a_val == NULL) {
        a_val = (seq_t*)malloc(sizeof(seq_t));
        c_val = (long long*)malloc(sizeof(long long));
        if (!a_val || !c_val) return 0;   /* memory error */
        a_val[0] = (seq_t)1;
        c_val[0] = 0;
        cap = 0;
    }

    /* extend memoisation if necessary */
    if (n > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp_a = (seq_t*)realloc(a_val, new_cap * sizeof(seq_t));
        if (!tmp_a) return 0;   /* memory error */
        a_val = tmp_a;
        long long *tmp_c = (long long*)realloc(c_val, new_cap * sizeof(long long));
        if (!tmp_c) return 0;   /* memory error */
        c_val = tmp_c;

        /* compute c(k) for k=cap+1 .. n */
        for (idx_t k = cap + 1; k <= n; ++k) {
            long long ck = 0;
            idx_t d;
            for (d = 1; d * d <= k; ++d) {
                if (k % d == 0) {
                    int b1 = period6_b(d);
                    ck += (long long)d * b1;
                    idx_t d2 = k / d;
                    if (d2 != d) {
                        int b2 = period6_b(d2);
                        ck += (long long)d2 * b2;
                    }
                }
            }
            c_val[k] = ck;
        }

        /* compute a(i) for i=cap+1 .. n using the Euler transform recurrence */
        for (idx_t i = cap + 1; i <= n; ++i) {
            long long sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                unsigned long long aval = (unsigned long long)a_val[i - k];
                sum += c_val[k] * (long long)aval;
            }
            /* division is exact by definition of the transform */
            a_val[i] = (seq_t)(sum / (long long)i);
        }

        cap = n;
    }

    return a_val[n];
}
