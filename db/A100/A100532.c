/*
 * A100532  The first four numbers of this sequence are the primes 2,3,5,7.
 *          The other terms are calculated by adding the previous four terms.
 * https://oeis.org/A100532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A100532: a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4), a(1)=2, a(2)=3, a(3)=5, a(4)=7 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *memo = NULL;
    static size_t memo_len = 0;
    static idx_t memo_max = 0;

    if (n < 1)
        return 0;

    size_t target = (size_t)n;

    /* initialise cache if empty */
    if (memo == NULL) {
        memo_len = 4;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        if (memo == NULL) abort();          /* memory failure */
        memo[0] = 2;                         /* a(1) */
        memo[1] = 3;                         /* a(2) */
        memo[2] = 5;                         /* a(3) */
        memo[3] = 7;                         /* a(4) */
        memo_max = 4;
    }

    /* grow the array if necessary */
    if (target > memo_len) {
        size_t new_len = target;
        seq_t *tmp = (seq_t *)realloc(memo, new_len * sizeof(seq_t));
        if (tmp == NULL) abort();           /* memory failure */
        memo = tmp;
        memo_len = new_len;
    }

    /* compute missing values up to n */
    if (target > memo_max) {
        for (size_t i = memo_max + 1; i <= target; ++i) {
            memo[i - 1] = memo[i - 2] + memo[i - 3] + memo[i - 4] + memo[i - 5];
        }
        memo_max = target;
    }

    return memo[n - 1];
}
