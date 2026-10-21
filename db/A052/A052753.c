/*
 * A052753  Expansion of e.g.f.: log(1-x)^4.
 * https://oeis.org/A052753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 24*A000454(n) = 24*Stirling1(n,4) Recurrence for Stirling
 *            numbers of the first kind: c(n,0)=0 (n>0), c(0,0)=1
 *            c(n,k)=c(n-1,k-1)+(n-1)*c(n-1,k) Initial terms: a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=0, a(4)=24 …
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: memoized values of the sequence */
    static seq_t *values = NULL;          /* a[0..computed]      */
    static idx_t values_cap = 0;          /* current capacity    */
    static idx_t values_len = 0;          /* highest index filled */

    /* previous row of Stirling numbers: c_prev[k] = c(i-1,k) */
    static seq_t c_prev[5];
    static seq_t c_next[5];

    /* initialise on first call */
    if (!values) {
        values_cap = 32;
        values = (seq_t *)malloc(values_cap * sizeof(seq_t));
        if (!values) return (seq_t)0;          /* should never happen */
        values[0] = (seq_t)0;                  /* a(0) = 0           */
        values_len = 0;
        c_prev[0] = (seq_t)1;                  /* c(0,0) = 1         */
        for (int k = 1; k <= 4; ++k) c_prev[k] = (seq_t)0;
    }

    /* grow array if necessary */
    if (n >= values_cap) {
        idx_t new_cap = values_cap;
        while (n >= new_cap) new_cap <<= 1;    /* double until enough */
        seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;             /* allocation failed */
        values = tmp;
        values_cap = new_cap;
    }

    /* compute missing values up to n (in any order of calls) */
    for (idx_t i = values_len + 1; i <= n; ++i) {
        /* compute c_next from c_prev for index i */
        c_next[0] = (seq_t)0;                  /* c(i,0)=0 for i>0    */
        for (int k = 1; k <= 4; ++k) {
            c_next[k] = c_prev[k-1] + ((seq_t)(i-1))*c_prev[k];
        }

        /* store a[i] = 24 * c_next[4] */
        values[i] = (seq_t)24 * c_next[4];

        /* prepare for next iteration */
        for (int k = 0; k <= 4; ++k) c_prev[k] = c_next[k];
    }

    values_len = n;
    return values[n];
}
