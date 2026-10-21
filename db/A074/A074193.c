/*
 * A074193  Sum of determinants of 2nd order principal minors of powers of the
 *          matrix ((1,1,0,0),(1,0,1,0),(1,0,0,1),(1,0,0,0)).
 * https://oeis.org/A074193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1)-2a(n-2)-2a(n-3)+2a(n-4)-a(n-5)+a(n-6); Initial
 *            values: a(0)=6, a(1)=-1, a(2)=-3, a(3)=-1, a(4)=17, a(5)=-16,
 *            a(6)=-15
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 46, i64 n <= 95, i128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static size_t memo_size = 0;
    static const seq_t init[7] = {(seq_t)6, (seq_t)-1, (seq_t)-3, (seq_t)-1,
                                  (seq_t)17, (seq_t)-16, (seq_t)-15};

    /* Initialise memoiser with the first seven terms */
    if (!memo) {
        memo = (seq_t *)malloc(sizeof(seq_t) * 7);
        for (size_t i = 0; i < 7; ++i)
            memo[i] = init[i];
        memo_size = 7;
    }

    /* Extend memoisation up to n if necessary */
    if ((size_t)n + 1 > memo_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_mem = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        if (!new_mem)
            return (seq_t)0;          /* Memory failure – unlikely in tests */
        memo = new_mem;

        for (size_t i = memo_size; i < new_size; ++i) {
            seq_t v = -memo[i-1] - (seq_t)2 * memo[i-2] - (seq_t)2 * memo[i-3]
                      + (seq_t)2 * memo[i-4] - memo[i-5] + memo[i-6];
            memo[i] = v;
        }
        memo_size = new_size;
    }
    return memo[n];
}
