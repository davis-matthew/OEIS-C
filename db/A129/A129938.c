/*
 * A129938  "Self-Fibonacci"; a(n) is the sum of the last nine terms. Sequence
 *          starts with 6,9,2,15,14,1,3,3,9 which are f,i,b,o,n,a,c,c,i if you
 *          consider a=1, b=2, c=3, ..., z=26.
 * https://oeis.org/A129938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = a(n-1) + a(n-2) + … + a(n-9)   (for n > 9)
   a(1)=6, a(2)=9, a(3)=2, a(4)=15, a(5)=14,
   a(6)=1, a(7)=3, a(8)=3, a(9)=9
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;       /* array of terms, 1‑indexed */
    static idx_t seq_len = 0;       /* number of terms stored (>=9 after init) */
    static idx_t seq_cap = 0;       /* capacity of seq array */

    /* initialise with the first nine terms */
    if (seq_len == 0) {
        seq_cap = 9;
        seq_len = 9;
        seq = (seq_t *)malloc((seq_cap + 1) * sizeof(seq_t));
        if (!seq) return 0; /* allocation failed; unlikely in test */
        seq[1] = 6;
        seq[2] = 9;
        seq[3] = 2;
        seq[4] = 15;
        seq[5] = 14;
        seq[6] = 1;
        seq[7] = 3;
        seq[8] = 3;
        seq[9] = 9;
    }

    /* extend cache if necessary */
    if (n > seq_len) {
        /* enlarge array to size n */
        if (n > seq_cap) {
            seq = (seq_t *)realloc(seq, (n + 1) * sizeof(seq_t));
            if (!seq) return 0; /* allocation failed; unlikely in test */
            seq_cap = n;
        }
        /* compute terms from seq_len+1 up to n */
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t j = i - 9; j < i; ++j) {
                sum += seq[j];
            }
            seq[i] = sum;
        }
        seq_len = n;
    }

    return seq[n];
}
