/*
 * A163877  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A163877
   Formula: a(0)=1, a(1)=4, a(2)=12, a(3)=36, a(4)=108, a(5)=324
   For n≥6: a(n)=2*a(n-1)+2*a(n-2)+2*a(n-3)+2*a(n-4)+2*a(n-5)-3*a(n-6) + (n==6?1:0)
   (t^6+2t^5+2t^4+2t^3+2t^2+2t+1)/(3t^6-2t^5-2t^4-2t^3-2t^2-2t+1)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)36;
    if (n == 4) return (seq_t)108;
    if (n == 5) return (seq_t)324;
    /* For n ≥ 6 we memoise the results */
    static seq_t *memo = NULL;
    static size_t memo_size = 0;
    static idx_t memo_last = 5;                /* highest index already computed */

    /* Ensure the memoisation array is large enough */
    if (n >= memo_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure – unlikely in tests */
        memo = tmp;
        memo_size = new_size;
        /* initialise seed values if this is the first allocation */
        if (memo_size >= 6) {
            memo[0] = (seq_t)1;
            memo[1] = (seq_t)4;
            memo[2] = (seq_t)12;
            memo[3] = (seq_t)36;
            memo[4] = (seq_t)108;
            memo[5] = (seq_t)324;
        }
    }

    /* Compute any missing values up to n */
    for (idx_t i = memo_last + 1; i <= n; ++i) {
        seq_t val = (seq_t)2 * memo[i-1]
                  + (seq_t)2 * memo[i-2]
                  + (seq_t)2 * memo[i-3]
                  + (seq_t)2 * memo[i-4]
                  + (seq_t)2 * memo[i-5]
                  - (seq_t)3 * memo[i-6];
        if (i == 6) val += (seq_t)1;   /* p_6 = 1 from the numerator */
        memo[i] = val;
    }
    memo_last = n;
    return memo[n];
}
