/*
 * A097864  Matrix recurrence A[n] = M * A[n-1] with A[0] =
 *          [[0,1,1],[1,1,2],[1,2,4]] and M = [[0,1,0],[0,1,0],[1,1,1]],
 *          flattened.
 * https://oeis.org/A097864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2*a(n-9) - a(n-18) for n >= 27, with initial values
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=2, a(6)=1, a(7)=2,
 *            a(8)=4, a(9)=1, a(10)=1, a(11)=2, a(12)=1, a(13)=1, a(14)=2,
 *            a(15)=2, a(16)=4, a(17)=7, a(18)=1, a(19)=1, a(20)=2, a(21)=1,
 *            a(22)=1, a(23)=2, a(24)=4, a(25)=6, a(26)=11. (Val...
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
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
    static seq_t *seq = NULL;
    static size_t seq_len = 0;   /* number of terms already stored */
    static size_t seq_cap = 0;   /* capacity of the allocated buffer */

    /* ---------- lazy initialization of the first 27 terms ---------- */
    if (seq == NULL) {
        /* we need at least 27 terms (indices 0..26) */
        seq_cap = 27 * 9;           /* 27 matrices of 9 values each, sufficient capacity */
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) {
            /* if allocation fails, return 0 (should never happen in contest) */
            return (seq_t)0;
        }

        /* A[0] matrix */
        seq_t A[9] = {
            (seq_t)0, (seq_t)1, (seq_t)1,   /* first row   */
            (seq_t)1, (seq_t)1, (seq_t)2,   /* second row  */
            (seq_t)1, (seq_t)2, (seq_t)4    /* third row   */
        };

        /* copy A[0] into the sequence (indices 0..8) */
        for (size_t i = 0; i < 9; ++i)
            seq[i] = A[i];
        seq_len = 9;   /* currently stored 9 terms: 0..8 */

        /* generate A[1] .. A[26] using the recurrence
           A_next[0..2]  = A_cur[3..5]   (second row)
           A_next[3..5]  = A_cur[3..5]   (second row again)
           A_next[6]     = A_cur[0] + A_cur[3] + A_cur[6];
           A_next[7]     = A_cur[1] + A_cur[4] + A_cur[7];
           A_next[8]     = A_cur[2] + A_cur[5] + A_cur[8];
        */
        for (size_t mat = 1; mat <= 26; ++mat) {
            seq_t next[9];
            next[0] = A[3];
            next[1] = A[4];
            next[2] = A[5];
            next[3] = A[3];
            next[4] = A[4];
            next[5] = A[5];
            next[6] = A[0] + A[3] + A[6];
            next[7] = A[1] + A[4] + A[7];
            next[8] = A[2] + A[5] + A[8];

            /* copy the 9 new values into seq */
            for (size_t i = 0; i < 9; ++i)
                seq[seq_len + i] = next[i];
            seq_len += 9;

            /* copy next matrix into A for the next iteration */
            for (size_t i = 0; i < 9; ++i)
                A[i] = next[i];
        }
        /* seq_len is now 27*9 = 243; we have all values a(0) .. a(242). */
    }

    /* ---------- handle requested index n ---------- */
    if ((size_t)n < seq_len)
        return seq[(size_t)n];

    /* need to extend the table up to n; recurrence holds for n >= 27 */
    for (size_t idx = seq_len; idx <= (size_t)n; ++idx) {
        /* compute a(idx) = 2*a(idx-9) - a(idx-18) */
        seq_t val = (seq_t)((seq_t)2 * seq[idx - 9] - seq[idx - 18]);

        /* ensure capacity */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap * 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!tmp) {
                /* on allocation failure just return 0 (should not happen) */
                return (seq_t)0;
            }
            seq = tmp;
            seq_cap = new_cap;
        }

        seq[seq_len++] = val;
    }

    return seq[(size_t)n];
}
