/*
 * A212750  Number of (w,x,y,z) with all terms in {0,...,n} and at least one
 *          of these conditions holds: w=R, x=R, y<R, z<R, where R =
 *          max{w,x,y,z} - min{w,x,y,z}.
 * https://oeis.org/A212750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212750: Number of (w,x,y,z) with all terms in {0,…,n}
   and at least one of the conditions w=R, x=R, y<R, z<R holds,
   where R = max{w,x,y,z} - min{w,x,y,z}.  Recurrence (n≥8):
       a(n) = 2*a(n-1)+2*a(n-2)-6*a(n-3)+6*a(n-5)-2*a(n-6)-2*a(n-7)+a(n-8)
   Initial values:
       a(0)=1, a(1)=14, a(2)=72, a(3)=216, a(4)=529, a(5)=1070,
       a(6)=1984, a(7)=3336
   The function memoises results so calls can be in any order.
*/
#include <stddef.h> /* for size_t */

seq_t a(idx_t n) {
    /* maximum number of terms we will handle (about 10 000). */
    enum { MAX_TERMS = 20001 };
    static seq_t cache[MAX_TERMS];
    /* marks whether cache[0]…cache[max_computed] have been filled */
    static size_t max_computed = (size_t)-1;

    /* constants for the first 8 terms */
    static const seq_t init[8] = {
        1, 14, 72, 216, 529, 1070, 1984, 3336
    };

    /* initialize cache on first call or if needed */
    if (max_computed == (size_t)-1) {
        for (idx_t i = 0; i < 8; ++i) {
            cache[i] = init[i];
        }
        max_computed = 7;
    }

    /* grow cache if the request exceeds our allocated size */
    if ((size_t)n + 1 > MAX_TERMS) {
        /* for safety still handle too large n by allocating exact size */
        /* but the problem guarantees n ≲ 10000, so this is unlikely.
           A usual implementation would use dynamic allocation, but
           the fixed-size array keeps the code simple and portable to
           both C and C++.  */
    }

    /* if we already have value for n, return it */
    if ((size_t)n <= max_computed) {
        return cache[n];
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = 2*cache[i-1] + 2*cache[i-2]
                   - 6*cache[i-3] + 6*cache[i-5]
                   - 2*cache[i-6] - 2*cache[i-7] + cache[i-8];
    }
    max_computed = n;
    return cache[n];
}
