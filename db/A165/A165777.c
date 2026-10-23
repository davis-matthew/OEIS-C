/*
 * A165777  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   p_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165777:  number of reduced words of length n in the
   Coxeter group on 6 generators with (S_i)^2 = (S_iS_j)^10 = I.
   The generating function is

        (t^10 + 2t^9 + … + 2t + 1) /
        (10t^10 - 4t^9 - … - 4t + 1)

   From this we obtain the linear recurrence (with a(−k)=0):

        a(n) = p_n + 4·a(n‑1)+4·a(n‑2)+…+4·a(n‑9) – 10·a(n‑10)

   where

        p_n = 1   if n==0 or n==10
        p_n = 2   if 1 <= n <= 9
        p_n = 0   otherwise.
*/

#include <stdlib.h>
#include "../utils/p_n.h"

/* allocate or grow the memoisation table up to index n */
static seq_t *seq = NULL;          /* array of computed terms */
static size_t seq_len = 0;         /* number of valid entries in seq */

seq_t a(idx_t n)
{
    /* grow memoisation table if needed */
    if ((size_t)n >= seq_len)
    {
        size_t new_len = (size_t)n + 1;
        seq = (seq_t *)realloc(seq, new_len * sizeof(seq_t));
        for (size_t i = seq_len; i < new_len; ++i)
        {
            seq_t sum = (seq_t)0;
            /* add 4 * a(i-k) for k = 1 .. 9  (if i>=k) */
            if (i >= 1) sum += (seq_t)4 * seq[i-1];
            if (i >= 2) sum += (seq_t)4 * seq[i-2];
            if (i >= 3) sum += (seq_t)4 * seq[i-3];
            if (i >= 4) sum += (seq_t)4 * seq[i-4];
            if (i >= 5) sum += (seq_t)4 * seq[i-5];
            if (i >= 6) sum += (seq_t)4 * seq[i-6];
            if (i >= 7) sum += (seq_t)4 * seq[i-7];
            if (i >= 8) sum += (seq_t)4 * seq[i-8];
            if (i >= 9) sum += (seq_t)4 * seq[i-9];
            /* subtract 10 * a(i-10) (if i>=10) */
            if (i >= 10) sum -= (seq_t)10 * seq[i-10];
            /* final value */
            seq[i] = p_n((idx_t)i) + sum;
        }
        seq_len = new_len;
    }
    return seq[(size_t)n];
}
