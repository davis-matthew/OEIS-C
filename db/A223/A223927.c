/*
 * A223927  Number of n X 2 0..2 arrays with rows, columns and antidiagonals
 *          unimodal and diagonals nondecreasing.
 * https://oeis.org/A223927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 47, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS entry A223927 */
/* Recurrence: a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *              126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)
 * Initial values for n=1..9:
 *  a(1)=9,  a(2)=54,  a(3)=218,  a(4)=698,  a(5)=1915,
 *  a(6)=4690, a(7)=10511, a(8)=21919, a(9)=43045.
 * This implementation memoises the sequence up to the largest n
 * requested, allowing calls in any order.
 */
#include <stdlib.h>
#include <stdint.h>

static seq_t *seq = NULL;      /* pointer to memoised array (index 1..n) */
static idx_t  seq_size = 0;    /* current maximum n stored (>=0) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset 1, but return 0 */
    /* initialise the table if not already */
    if (seq_size == 0) {
        seq_size = 9;
        seq = (seq_t*)malloc(sizeof(seq_t) * (seq_size + 1)); /* +1 for 1‑based */
        seq[1] = (seq_t)9;
        seq[2] = (seq_t)54;
        seq[3] = (seq_t)218;
        seq[4] = (seq_t)698;
        seq[5] = (seq_t)1915;
        seq[6] = (seq_t)4690;
        seq[7] = (seq_t)10511;
        seq[8] = (seq_t)21919;
        seq[9] = (seq_t)43045;
    }
    /* extend the table if necessary */
    if ((idx_t)n > seq_size) {
        idx_t new_size = (idx_t)n;
        seq_t *new_seq = (seq_t*)realloc(seq, sizeof(seq_t) * (new_size + 1));
        if (!new_seq) /* allocation failed */
            return 0;
        seq = new_seq;
        /* compute missing terms */
        for (idx_t i = seq_size + 1; i <= new_size; ++i) {
            seq[i] =  (seq_t)9  * seq[i-1]
                    - (seq_t)36 * seq[i-2]
                    + (seq_t)84 * seq[i-3]
                    - (seq_t)126 * seq[i-4]
                    + (seq_t)126 * seq[i-5]
                    - (seq_t)84 * seq[i-6]
                    + (seq_t)36 * seq[i-7]
                    - (seq_t)9  * seq[i-8]
                    + seq[i-9];
        }
        seq_size = new_size;
    }
    return seq[n];
}
