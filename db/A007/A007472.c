/*
 * A007472  Shifts 2 places left when binomial transform is applied twice with
 *          a(0) = a(1) = 1.
 * https://oeis.org/A007472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n+2) = Sum_{k=0..n} binomial(n,k) * 2^(n-k) * a(k), a(0) =
 *            a(1) = 1 (A applied twice by the binomial transform gives the
 *            sequence shifted left by 2). Computed using a difference-table
 *            anti-diagonal d(j+1,k) = 2*d(j,k) + d(j,k+1), d(0,k) = a(k), so
 *            that a(n+2) = d(n,0)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

seq_t a(idx_t n) {
    static seq_t *av = NULL;   /* known terms */
    static seq_t *D = NULL;    /* current anti-diagonal: D[j] = d(j, m-j) */
    static idx_t cap = 0;
    static idx_t known = 0;    /* number of terms in av */
    static idx_t absorbed = 0; /* number of terms folded into D */

    if (av == NULL) {
        cap = 16;
        av = alloc_zero(cap);
        D = alloc_zero(cap);
        av[0] = 1;
        av[1] = 1;
        known = 2;
        absorbed = 0;
    }

    while (known <= n) {
        if (known + 1 >= cap) {
            idx_t newcap = cap * 2;
            av = grow_zero(av, cap, newcap);
            D = grow_zero(D, cap, newcap);
            cap = newcap;
        }
        /* absorb av[m], m = absorbed: D has length m, becomes length m+1 */
        idx_t m = absorbed;
        seq_t old = D[0];
        D[0] = av[m];
        for (idx_t j = 0; j < m; j++) {
            seq_t tmp = D[j + 1];
            D[j + 1] = old * 2 + D[j];
            old = tmp;
        }
        absorbed = m + 1;
        av[known] = D[m];
        known++;
    }

    return av[n];
}
