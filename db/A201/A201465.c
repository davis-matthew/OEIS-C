/*
 * A201465  E.g.f. A(x) satisfies A(x) = (x + 2*exp(A(x)) - 2)/3.
 * https://oeis.org/A201465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) satisfies A(x) = (x + 2*exp(A(x)) - 2)/3. The sequence a(n)
 *            is defined by a(1)=1 and for n≥2 a(n)=2*∑_{j=1}^{n-1}
 *            binom(n-1,j) · a(n-j) · b(j), where b(j) is the j‑th coefficient
 *            of exp(A(x)); this recurrence follows from the functional
 *            equation and the relation between A and exp(A)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t computed = 0;   /* largest n for which a[ ] and b[ ] are known */
    static seq_t *a_vals = NULL; /* array a[1..] */
    static seq_t *b_vals = NULL; /* array b[0..] where b[0]=1, b[1]=a[1] */
    static size_t capacity = 0;   /* current allocation size */

    if (n == 0) return (seq_t)0; /* offset is 1, 0 is undefined */

    /* first allocation */
    if (a_vals == NULL) {
        capacity = 2; /* enough for indices 0 and 1 */
        a_vals = (seq_t *)malloc(capacity * sizeof(seq_t));
        b_vals = (seq_t *)malloc(capacity * sizeof(seq_t));
        /* initialise b[0] */
        b_vals[0] = (seq_t)1;
    }

    /* initialise the first term if not yet done */
    if (computed < 1) {
        a_vals[1] = (seq_t)1;
        b_vals[1] = (seq_t)1;
        computed = 1;
    }

    /* grow arrays if needed */
    if (n > (idx_t)(capacity - 1)) {
        size_t new_cap = (size_t)(n + 1);
        a_vals = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        b_vals = (seq_t *)realloc(b_vals, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* compute terms up to n if they are missing */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t S = (seq_t)0;                     /* the sum in the recurrence */
            idx_t n1 = i - 1;                       /* used in binomial updates */
            seq_t comb = (seq_t)n1;                 /* (i-1 choose 1) */

            for (idx_t j = 1; j <= n1; ++j) {
                seq_t term = comb * a_vals[i - j];   /* comb * a[i-j] */
                term = term * b_vals[j];            /* * b[j] */
                S += term;                          /* accumulate */
                if (j < n1) {                       /* prepare next binomial */
                    comb = comb * (seq_t)(n1 - j);
                    comb = comb / (seq_t)(j + 1);
                }
            }

            a_vals[i] = S + S;       /* a[i] = 2*S */
            b_vals[i] = S + S + S;   /* b[i] = 3*S = (3/2)*a[i] */
        }
        computed = n;
    }

    return a_vals[n];
}
