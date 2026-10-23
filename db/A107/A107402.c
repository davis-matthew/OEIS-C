/*
 * A107402  a(n)= -a(n-1) +5*a(n-2) +5*a(n-3) -a(n-4) -a(n-5).
 * https://oeis.org/A107402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -a(n-1)+5*a(n-2)+5*a(n-3)-a(n-4)-a(n-5); a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *memo = NULL;         /* memoized values */
static idx_t memo_len = 0;         /* number of elements currently stored */

seq_t a(idx_t n)
{
    /* Allocate or grow the memo table if necessary */
    if (n + 1 > memo_len) {
        idx_t new_len = n + 1;
        if (new_len < memo_len * 2) new_len = memo_len * 2;
        /* realloc returns void*, cast to seq_t* */
        seq_t *tmp = (seq_t*)realloc(memo, new_len * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure – return sentinel */
        memo = tmp;

        /* Compute new entries */
        for (idx_t i = memo_len; i < new_len; ++i) {
            if (i == 0)               memo[i] = (seq_t)0;
            else if (i == 1)          memo[i] = (seq_t)1;
            else if (i == 2)          memo[i] = (seq_t)1;
            else if (i == 3)          memo[i] = (seq_t)2;
            else if (i == 4)          memo[i] = (seq_t)3;
            else
                memo[i] = -memo[i-1]
                           + (seq_t)5 * memo[i-2]
                           + (seq_t)5 * memo[i-3]
                           - memo[i-4]
                           - memo[i-5];
        }
        memo_len = new_len;
    }
    return memo[n];
}
