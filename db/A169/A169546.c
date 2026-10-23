/*
 * A169546  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(n)=4*a(n-1) for 2≤n≤34 * Recurrence for all
 *            n≥1: * a(n) = 4*a(n-1) - 9*a(n-35) + 6*a(n-36) + b(n) * where
 *            b(1)=1, b(35)=-1, b(36)=-1, and b(n)=0 otherwise. * This
 *            implements the OGF (1+x)*(1-x^35)/(1-4x+9x^35-6x^36)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* persistent buffer for memoised values */
    static seq_t *buf = NULL;
    static idx_t capacity = 0;   /* number of allocated slots */
    static idx_t computed = 0;   /* highest n already computed (incl. 0) */

    /* allocate initial slot if needed */
    if (!buf) {
        buf = (seq_t*)malloc(sizeof(seq_t));
        if (!buf) abort();          /* out of memory */
        capacity = 1;
    }

    /* extend buffer if we need to go beyond current capacity */
    if (n + 1 > capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(buf,
                                     (size_t)new_cap * sizeof(seq_t));
        if (!tmp) { free(buf); abort(); }
        /* initialise the newly allocated slots to zero (unnecessary for
           recurrence but keeps the array well‑defined). */
        for (idx_t i = capacity; i < new_cap; ++i) tmp[i] = (seq_t)0;
        buf = tmp;
        capacity = new_cap;
    }

    /* initialise a(0) once */
    if (computed == 0) {
        buf[0] = (seq_t)1;
    }

    /* compute missing values up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t val = (seq_t)4 * buf[k - 1];

        if (k >= 35) val -= (seq_t)9 * buf[k - 35];
        if (k >= 36) val += (seq_t)6 * buf[k - 36];

        /* add the b(k) term */
        if (k == 1)   val += (seq_t)1;
        if (k == 35)  val -= (seq_t)1;
        if (k == 36)  val -= (seq_t)1;

        buf[k] = val;
    }

    computed = n;
    return buf[n];
}
