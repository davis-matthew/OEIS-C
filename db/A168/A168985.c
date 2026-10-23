/*
 * A168985  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A168985: number of reduced words of length n in Coxeter group on 20 generators
 *  (S_i)^2 = I, (S_i S_j)^3 = I.
 *
 *  The ordinary generating function is
 *      G(t) = (t^23 + 2*t^22 + 2*t^21 + ... + 2*t + 1)
 *             ----------------------------------------
 *             1 - 18*t - 18*t^2 - ... - 18*t^22 + 171*t^23
 *
 *  From this one obtains the linear recurrence
 *      a(n) = 18 * (a(n-1)+...+a(n-22)) - 171 * a(n-23)   for n ≥ 23
 *  with initial values
 *      a(0)=1, a(1)=20, a(2)=380, a(3)=7220, a(4)=137180,
 *      a(5)=2606420, a(6)=49521980, a(7)=940917620,
 *      a(8)=17877434780, a(9)=339671260820, a(10)=6453753955580,
 *      a(11)=122621325156020, a(12)=2329805177964380,
 *      a(13)=44266298381323220, a(14)=841059669245141180,
 *      a(15)=15980133715657682420, a(16),...,a(22) are obtained
 *      automatically by the same recurrence using the numerator
 *      coefficients (p(n)=1 for n=0,23; p(n)=2 for 1≤n≤22; 0 otherwise).
 *
 *  The sequence is computed lazily and stored in a static cache for
 *  repeated unsorted calls.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache: a[0..sz-1] holds computed values */
    static seq_t *a_arr = NULL;
    static size_t sz   = 0;          /* number of elements stored (non‑zero) */

    /* enlarge cache if necessary */
    if (sz <= (size_t)n) {
        size_t new_sz = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_arr, new_sz * sizeof(seq_t));
        if (!tmp) { abort(); }       /* unrecoverable allocation failure */
        a_arr = tmp;

        /* fill missing entries */
        for (idx_t i = (idx_t)sz; i <= n; ++i) {
            size_t ii = (size_t)i;   /* array index */
            /* sum of previous 22 terms */
            seq_t sum = (seq_t)0;
            for (int k = 1; k <= 22; ++k) {
                if (ii >= (size_t)k) sum += a_arr[ii - k];
            }
            /* apply recurrence */
            seq_t term = sum * (seq_t)18;
            if (ii >= 23) term -= a_arr[ii - 23] * (seq_t)171;

            /* add numerator contribution p(i)  */
            seq_t p = (ii == 0 || ii == 23) ? (seq_t)1
                                            : ((ii <= 22) ? (seq_t)2 : (seq_t)0);
            term += p;

            a_arr[ii] = term;
        }
        sz = new_sz;
    }
    return a_arr[(size_t)n];
}
