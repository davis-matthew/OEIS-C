/*
 * A170459  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A170459: Number of reduced words of length n in
 * the Coxeter group with 18 generators and relations (S_i)^2 = (S_i S_j)^45 = I.
 *
 * The growth series satisfies the rational generating function
 *
 *   G(t) = (t^45 + 2∑_{k=1}^{44} t^k + 1) / (136 t^45 - 16∑_{k=1}^{44} t^k + 1).
 *
 * From the denominator we obtain the linear recurrence
 *
 *   a(0) = 1
 *   a(n) = 16·∑_{i=0}^{n-1} a(i) + 2          for 1 ≤ n ≤ 44
 *   a(n) = 16·∑_{i=n-44}^{n-1} a(i) - 136·a(n-45)   for n ≥ 45
 *
 * This implementation keeps a cache of computed terms so that any call
 * to a(idx_t n) runs in O(n) time with negligible overhead.
 */

#include <stddef.h>          /* for size_t */

seq_t a(idx_t n)
{
    /* static cache of values and prefix sums */
    static seq_t *values   = NULL;   /* a[i]   */
    static seq_t *prefix   = NULL;   /* prefix[i] = a[0] + … + a[i] */
    static idx_t  max_n    = 0;      /* number of terms allocated */

    /* Allocate or expand the cache if necessary */
    if (n >= max_n) {
        idx_t new_cap = n + 1;
        /* Use realloc in a portable way */
        values  = (seq_t *) realloc(values, new_cap * sizeof(seq_t));
        prefix  = (seq_t *) realloc(prefix, new_cap * sizeof(seq_t));
        /* For newly allocated slots, zero the prefix sum (will be set later) */
        max_n = new_cap;
    }

    /* Compute terms up to n if not already done */
    static idx_t computed = 0;       /* next index to compute, i.e. computed == values[computed-1] is defined */
    if (computed == 0) {             /* first time: initialise a[0] and prefix[0] */
        values[0]  = (seq_t)1;
        prefix[0]  = (seq_t)1;
        computed   = 1;              /* 0 is now computed */
    }

    for (idx_t i = computed; i <= n; ++i) {
        if (i <= 44) {               /* recurrence for 1 <= i <= 44 */
            seq_t sum = prefix[i - 1];
            values[i] = sum * (seq_t)16 + (seq_t)2;
        } else {                     /* recurrence for i >= 45 */
            seq_t sum_prev44 = prefix[i - 1] - prefix[i - 45];
            values[i] = sum_prev44 * (seq_t)16 - values[i - 45] * (seq_t)136;
        }
        prefix[i] = prefix[i - 1] + values[i];
    }

    computed = n + 1;                /* all terms up to n are now computed */
    return values[n];
}
