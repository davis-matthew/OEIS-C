/*
 * A183685  Number of (n+1) X 7 binary arrays with every 2 X 2 subblock
 *          nonsingular.
 * https://oeis.org/A183685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)=192, a(2)=1360, a(3)=8704, a(4)=58448, a(5)=382848,
 *            a(6)=2542368, a(7)=16748160, a(8)=110871040
 *            a(n)=3*a(n-1)+48*a(n-2)-78*a(n-3)-720*a(n-4)+612*a(n-5)+4232*a(n-6)
 *            -1392*a(n-7)-8192*a(n-8) for n>=9 (recurrence with integer
 *            coefficients, all results are non‑negative)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoisation array holding all computed values */
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;          /* number of elements in memo */

    /* initial 8 constants */
    static const seq_t init[8] = {
        (seq_t)192,      (seq_t)1360,      (seq_t)8704,
        (seq_t)58448,    (seq_t)382848,    (seq_t)2542368,
        (seq_t)16748160, (seq_t)110871040
    };

    /* initialise memo if first call */
    if (memo == NULL) {
        memo_len = 8;
        memo = (seq_t *)malloc(sizeof(seq_t) * memo_len);
        if (memo == NULL) return 0; /* allocation failure, return zero */
        for (idx_t i = 0; i < memo_len; ++i)
            memo[i] = init[i];
    }

    if (n <= memo_len)          /* already computed */
        return memo[n - 1];

    /* extend memo to hold n elements */
    if (n > memo_len) {
        seq_t *new_mem = (seq_t *)realloc(memo, sizeof(seq_t) * n);
        if (new_mem == NULL) return 0; /* allocation failure */
        memo = new_mem;
        /* compute missing values */
        for (idx_t i = memo_len; i < n; ++i) {
            /* indices are 0‑based: a(k) is memo[k‑1] */
            seq_t plus  = (seq_t)3   * memo[i - 1]
                        + (seq_t)48  * memo[i - 2]
                        + (seq_t)612 * memo[i - 5]
                        + (seq_t)4232 * memo[i - 6];
            seq_t minus = (seq_t)78  * memo[i - 3]
                        + (seq_t)720 * memo[i - 4]
                        + (seq_t)1392 * memo[i - 7]
                        + (seq_t)8192 * memo[i - 8];
            memo[i] = plus - minus;
        }
        memo_len = n;
    }

    return memo[n - 1];
}
