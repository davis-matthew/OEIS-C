/*
 * A052896  E.g.f.: (exp(exp(x)-1)-1)^2.
 * https://oeis.org/A052896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{k=0..n} Stirling2(n,k)·(2^k-2), n≥1 (a(0)=0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t  *Sarr   = NULL; /* current Stirling array S(n,·) */
static size_t Slen   = 0;      /* size of Sarr  */
static seq_t  *aval   = NULL; /* computed a(n) values */
static size_t Alen   = 0;      /* number of stored a(n) values (≥1) */
static idx_t   computed = 0;   /* largest n for which a(n) is known */

seq_t a(idx_t n)
{
    /* Lazy initialization for n = 0 */
    if (aval == NULL || Sarr == NULL) {
        /* a(0) = 0 */
        aval  = (seq_t*)malloc(sizeof(seq_t));
        aval[0] = (seq_t)0;
        Alen = 1;

        /* S(0,0) = 1  */
        Sarr = (seq_t*)malloc(sizeof(seq_t));
        Sarr[0] = (seq_t)1;
        Slen = 1;

        computed = 0;
    }

    if (n == 0) return aval[0];
    if (n <= computed) return aval[n];

    /* Extend precomputation up to n */
    for (idx_t cur = computed + 1; cur <= n; ++cur) {
        size_t cur_len = (size_t)cur + 1;          /* new array length */

        seq_t *newS = (seq_t*)malloc(sizeof(seq_t) * cur_len);
        if (!newS) return (seq_t)0;               /* allocation failure (unlikely) */

        /* S(cur,0) = 0 */
        newS[0] = (seq_t)0;

        /* compute S(cur,k) for k = 1 .. cur using recurrence */
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term1 = (k-1 < Slen) ? Sarr[k-1] : (seq_t)0; /* S(cur-1,k-1) */
            seq_t term2 = (k <   Slen) ? Sarr[k]   : (seq_t)0; /* S(cur-1,k)   */
            newS[k] = term1 + ((seq_t)k) * term2;
        }

        /* evaluate a(cur) = Σ_{k=1..cur} S(cur,k)*(2^k-2) */
        seq_t a_cur = (seq_t)0;
        seq_t pow2  = (seq_t)1; /* 2^0 */
        for (idx_t k = 1; k <= cur; ++k) {
            pow2 = pow2 * (seq_t)2;                /* 2^k */
            seq_t diff = pow2 - (seq_t)2;          /* 2^k-2  */
            a_cur = a_cur + newS[k] * diff;
        }

        /* store the new a(cur) value */
        aval = (seq_t*)realloc(aval, sizeof(seq_t) * (cur + 1));
        if (!aval) return (seq_t)0;                /* allocation failure */
        aval[cur] = a_cur;
        Alen = cur + 1;

        /* replace the old Stirling array */
        free(Sarr);
        Sarr = newS;
        Slen = cur_len;

        computed = cur;
    }

    return aval[n];
}
