/*
 * A204103  Number of (n+1) X 6 0..2 arrays with column and row pair sums
 *          b(i,j)=a(i,j)+a(i,j-1) and c(i,j)=a(i,j)+a(i-1,j) such that
 *          b(i,j)*b(i-1,j)-c(i,j)*c(i,j-1) is nonzero.
 * https://oeis.org/A204103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 6‑term linear recurrence a(n) = 25*a(n-1) -
 *            45*a(n-2) - 963*a(n-3) + 2025*a(n-4) + 3645*a(n-5) - 6561*a(n-6)
 *            with a(1) = 9216, a(2) = 186624, a(3) = 3779136, a(4) =
 *            78428736, a(5) = 1631513664, a(6) = 34026967296
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms, a(1) stored at memo[0] */
    static seq_t *memo  = NULL;
    static idx_t  cap   = 0;      /* capacity of memo array            */
    static idx_t  count = 0;      /* number of terms actually stored   */

    /* trivial for small n */
    if (n < 1)                 return 0;
    if (n <= 6)                return (seq_t[]){9216,186624,3779136,78428736,1631513664,34026967296}[n-1];

    /* initialise cache on first call */
    if (!memo) {
        cap   = 16;
        memo  = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!memo) exit(1);           /* just in case */
        memo[0] = (seq_t)9216;
        memo[1] = (seq_t)186624;
        memo[2] = (seq_t)3779136;
        memo[3] = (seq_t)78428736;
        memo[4] = (seq_t)1631513664;
        memo[5] = (seq_t)34026967296;
        count = 6;
    }

    /* extend cache up to n */
    for (idx_t k = count + 1; k <= n; ++k) {
        if (count + 1 > cap) {
            cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(memo, cap * sizeof(seq_t));
            if (!tmp) exit(1);
            memo = tmp;
        }

        /* compute a(k) from a(k‑1)…a(k‑6) */
        seq_t value = (seq_t)25  * memo[k-2];  /* a(k-1) */
        value -= (seq_t)45  * memo[k-3];     /* a(k-2) */
        value -= (seq_t)963 * memo[k-4];     /* a(k-3) */
        value += (seq_t)2025 * memo[k-5];    /* a(k-4) */
        value += (seq_t)3645 * memo[k-6];    /* a(k-5) */
        value -= (seq_t)6561 * memo[k-7];    /* a(k-6) */
        memo[k-1] = value;
        count = k;
    }

    return memo[n-1];
}
