/*
 * A079816  Number of permutations satisfying -k <= p(i)-i <= r and p(i)-i not
 *          in I, i=1..n, with k=1, r=5, I={1}.
 * https://oeis.org/A079816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-3)+a(n-4)+a(n-5)+a(n-6) with initial values
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=4
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 42, u64 n <= 83, u128 n <= 165,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;   /* memoised table */
    static idx_t size = 0;     /* number of elements stored */

    if (n + 1 > size) {
        idx_t new_size = n + 1;
        /* realloc returns void *; cast explicitly for C++ */
        seq_t *newdp = (seq_t *)realloc(dp, new_size * sizeof(*dp));
        if (!newdp) abort();   /* allocation failure */

        if (dp == NULL) {            /* first allocation – initialise base cases */
            newdp[0] = 1;            /* a(0) */
            for (idx_t i = 1; i <= n; ++i) {
                seq_t sum = 0;
                if (i > 0) sum += newdp[i - 1];
                if (i > 2) sum += newdp[i - 3];
                if (i > 3) sum += newdp[i - 4];
                if (i > 4) sum += newdp[i - 5];
                if (i > 5) sum += newdp[i - 6];
                newdp[i] = sum;
            }
        } else {                     /* extend existing table */
            for (idx_t i = size; i <= n; ++i) {
                seq_t sum = 0;
                if (i > 0) sum += newdp[i - 1];
                if (i > 2) sum += newdp[i - 3];
                if (i > 3) sum += newdp[i - 4];
                if (i > 4) sum += newdp[i - 5];
                if (i > 5) sum += newdp[i - 6];
                newdp[i] = sum;
            }
        }

        dp   = newdp;
        size = new_size;
    }

    return dp[n];
}
