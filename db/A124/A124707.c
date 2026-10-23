/*
 * A124707  Number of base 14 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = trace(A^n) where A is a 14×14 matrix with A[i][j] = 1 if
 *            |i−j| ≤ 1, otherwise 0. a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;      /* memo[i] = a(i) */
    static size_t memo_cap = 0;
    static size_t memo_len = 0;

    /* one‑time initialization */
    if (memo == NULL) {
        memo_cap = 2;                     /* at least for n = 0 and n = 1 */
        memo     = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo) return (seq_t)0;       /* allocation failure */

        memo[0] = (seq_t)1;               /* a(0) */
        memo[1] = (seq_t)14;              /* a(1) = trace(A) */
        memo_len = 2;
    }

    /* matrix current state: cur = A^cur_step, starting with step = 1 */
    static seq_t cur[14][14];
    static seq_t next[14][14];
    static idx_t cur_step = 0;

    /* initialise cur to A^1 if this is the first time */
    if (cur_step == 0) {
        for (idx_t i = 0; i < 14; ++i) {
            for (idx_t j = 0; j < 14; ++j) {
                if (i == j || (j > 0 && i == j - 1) || (j + 1 < 14 && i == j + 1))
                    cur[i][j] = (seq_t)1;
                else
                    cur[i][j] = (seq_t)0;
            }
        }
        cur_step = 1;
    }

    /* extend precomputation up to requested n */
    while (cur_step < n) {
        /* next = cur * A   (A is tridiagonal with 1’s on diag and ±1) */
        for (idx_t i = 0; i < 14; ++i) {
            for (idx_t j = 0; j < 14; ++j) {
                seq_t val = cur[i][j];
                if (j > 0)      val += cur[i][j - 1];
                if (j + 1 < 14) val += cur[i][j + 1];
                next[i][j] = val;
            }
        }

        /* copy next to cur */
        for (idx_t i = 0; i < 14; ++i)
            for (idx_t j = 0; j < 14; ++j)
                cur[i][j] = next[i][j];

        /* trace of cur   -> a(cur_step+1) */
        seq_t tr = 0;
        for (idx_t i = 0; i < 14; ++i)
            tr += cur[i][i];

        /* enlarge memo if necessary */
        if (memo_len == memo_cap) {
            size_t new_cap = memo_cap * 2;
            seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);     /* abort on allocation failure */
            memo     = tmp;
            memo_cap = new_cap;
        }

        memo[memo_len++] = tr;   /* store a(cur_step+1) */
        ++cur_step;
    }

    return memo[n];
}
