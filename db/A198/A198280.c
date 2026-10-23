/*
 * A198280  Number of 2n X 6 0..2 arrays with values 0..2 introduced in row
 *          major order and each element equal to exactly two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A198280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(1)=2, a(2)=14, a(3)=100, a(4)=662, a(5)=4588, a(6)=31718,
   a(7)=219060, a(8)=1516006, a(9)=10490668
   a(n) = 8*a(n-1)-2*a(n-2)-20*a(n-3)-145*a(n-4)+68*a(n-5)
                      +492*a(n-6)+512*a(n-7)+64*a(n-8)-2048*a(n-9)   (n≥10)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *seq = NULL;   /* seq[0] = a(1) */
    static idx_t seq_len = 0;   /* number of terms stored */

    /* initialise first 9 terms on first call */
    if (seq_len == 0) {
        seq_len = 9;
        seq = (seq_t *)malloc(sizeof(seq_t) * seq_len);
        seq[0] = (seq_t)2;
        seq[1] = (seq_t)14;
        seq[2] = (seq_t)100;
        seq[3] = (seq_t)662;
        seq[4] = (seq_t)4588;
        seq[5] = (seq_t)31718;
        seq[6] = (seq_t)219060;
        seq[7] = (seq_t)1516006;
        seq[8] = (seq_t)10490668;
    }

    if (n <= seq_len) {
        return seq[n-1];
    }

    /* extend cache up to n */
    seq = (seq_t *)realloc(seq, sizeof(seq_t) * n);
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        seq_t val = 8 * seq[i-2];
        val -= 2 * seq[i-3];
        val -= 20 * seq[i-4];
        val -= 145 * seq[i-5];
        val += 68 * seq[i-6];
        val += 492 * seq[i-7];
        val += 512 * seq[i-8];
        val += 64 * seq[i-9];
        val -= 2048 * seq[i-10];
        seq[i-1] = val;
    }
    seq_len = n;
    return seq[n-1];
}
