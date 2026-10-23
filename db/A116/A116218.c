/*
 * A116218  If X_1,...,X_n is a partition of a 2n-set X into 2-blocks (or
 *          pairs) then a(n) is equal to the number of permutations f of X
 *          such that f(X_i) != X_i for all i=1,...n.
 * https://oeis.org/A116218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} (-2)^i * binomial(n,i) * (2*n-2*i)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0, a(2)=20;
   a(n) = 2*(n-1)*(2*n+1)*a(n-1) + 4*(n-1)*(4*n-3)*a(n-2) + 16*(n-2)*(n-1)*a(n-3) */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)20;

    /* Iterate recurrence */
    seq_t a_nm3 = (seq_t)1;   /* a(0) */
    seq_t a_nm2 = (seq_t)0;   /* a(1) */
    seq_t a_nm1 = (seq_t)20;  /* a(2) */
    seq_t a_n   = 0;          /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        /* term from a(i-1) */
        seq_t t1 = a_nm1;
        t1 *= (seq_t)2 * (seq_t)(i-1);
        t1 *= (seq_t)2 * (seq_t)i + (seq_t)1;

        /* term from a(i-2) */
        seq_t t2 = a_nm2;
        t2 *= (seq_t)4 * (seq_t)(i-1);
        t2 *= (seq_t)4 * (seq_t)i - (seq_t)3;

        /* term from a(i-3) */
        seq_t t3 = a_nm3;
        t3 *= (seq_t)16 * (seq_t)(i-2) * (seq_t)(i-1);

        a_n = t1 + t2 + t3;

        /* shift window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
