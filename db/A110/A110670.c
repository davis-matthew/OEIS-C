/*
 * A110670  Sequence is {a(5,n)}, where a(m,n) is defined at sequence A110665.
 * https://oeis.org/A110670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 28, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A110670
   Recurrence of order 9 derived from the empirical generating function
   x*(2*x-1)/((x-1)^5*(x^2-x+1)^2):
       a(n) =  7*a(n-1) + (-23)*a(n-2) + 47*a(n-3) + (-66)*a(n-4)
              + 66*a(n-5) + (-47)*a(n-6) + 23*a(n-7) + (-7)*a(n-8) + 1*a(n-9)
              - rhs(n)
   where rhs(n) = -1 for n = 1,  2 for n = 2,  0 otherwise.
   Initial value a(0) = 0 is forced by the recurrence with a(k)=0 for k<0.
   The function memoises computed terms and grows the buffer lazily.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *terms   = NULL;
    static idx_t last_computed = (idx_t)-1;      /* no terms computed yet */

    /* Helper to grow the cache, zeroing new entries */
    if (terms == NULL) {
        terms = (seq_t *)realloc(terms, (n + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= n; i++)
            terms[i] = (seq_t)0;  /* a(0)=0 is already 0 */
        last_computed = 0;
    } else if (n > last_computed) {
        terms = (seq_t *)realloc(terms, (n + 1) * sizeof(seq_t));
    } else if (n <= last_computed)
        return terms[n];

    /* compute new terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; i++) {
        seq_t sum = (seq_t)0;
        if (i >= 1) sum += (seq_t) 7  * terms[i - 1];
        if (i >= 2) sum += (seq_t)-23 * terms[i - 2];
        if (i >= 3) sum += (seq_t)47  * terms[i - 3];
        if (i >= 4) sum += (seq_t)-66 * terms[i - 4];
        if (i >= 5) sum += (seq_t)66  * terms[i - 5];
        if (i >= 6) sum += (seq_t)-47 * terms[i - 6];
        if (i >= 7) sum += (seq_t)23  * terms[i - 7];
        if (i >= 8) sum += (seq_t)-7  * terms[i - 8];
        if (i >= 9) sum += (seq_t) 1  * terms[i - 9];

        seq_t rhs = (seq_t)0;
        if (i == 1) rhs = (seq_t)-1;
        else if (i == 2) rhs = (seq_t)2;

        terms[i] = sum - rhs;
    }

    last_computed = n;
    return terms[n];
}
