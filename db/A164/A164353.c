/*
 * A164353  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -3*a(n-7) + 2*Sum_{k=1..6} a(n-k) Initial values: a(0)=1,
 *            a(1)=4, a(2)=12, a(3)=36, a(4)=108, a(5)=324, a(6)=972,
 *            a(7)=2910
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed terms */
    static seq_t *seq   = NULL;
    static idx_t  seq_len = 0;

    /* initialize cache with the first 8 terms */
    if (seq_len == 0) {
        seq_len = 8;
        seq = (seq_t*)malloc(seq_len * sizeof(seq_t));
        if (!seq) return 0;  /* allocation failure – unlikely for n≤10000 */

        seq[0] = (seq_t)1;
        seq[1] = (seq_t)4;
        seq[2] = (seq_t)12;
        seq[3] = (seq_t)36;
        seq[4] = (seq_t)108;
        seq[5] = (seq_t)324;
        seq[6] = (seq_t)972;
        seq[7] = (seq_t)2910;
    }

    /* grow the cache if necessary */
    if (n >= seq_len) {
        idx_t new_len = n + 1;
        seq = (seq_t*)realloc(seq, new_len * sizeof(seq_t));
        if (!seq) return 0;  /* allocation failure – unlikely for n≤10000 */

        /* compute missing terms using the recurrence */
        for (idx_t i = seq_len; i < new_len; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 6; ++k) {
                sum += seq[i - k];
            }
            /* 2*sum - 3*seq[i-7] is always non‑negative for i ≥ 8 */
            seq[i] = 2 * sum - 3 * seq[i - 7];
        }
        seq_len = new_len;
    }

    return seq[n];
}
