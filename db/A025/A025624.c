/*
 * A025624  Numbers of form 5^i*9^j, with i, j >= 0.
 * https://oeis.org/A025624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) are all numbers 5^i * 9^j sorted increasingly, with i,j ≥ 0
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 82, u64 n <= 303,
 *            u128 n <= 1161, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – n=0 is unused */

    /* Static state (memoisation) */
    static seq_t *seq  = NULL;
    static size_t seqCap = 0;
    static idx_t length   = 0;      /* number of terms currently stored */
    static idx_t ptr5     = 0;      /* next index for multiplying by 5   */
    static idx_t ptr9     = 0;      /* next index for multiplying by 9   */

    /* Initialise storage on first call */
    if (seq == NULL) {
        seqCap = 1;
        seq = (seq_t *)malloc(seqCap * sizeof(seq_t));
        if (seq == NULL) abort();
        seq[0] = (seq_t)1;    /* a(1) */
        length = 1;
        ptr5 = 0;
        ptr9 = 0;
    }

    /* Grow the array if necessary */
    if (seqCap < n) {
        size_t newCap = seqCap ? seqCap : 1;
        while (newCap < n) newCap <<= 1;
        seq = (seq_t *)realloc(seq, newCap * sizeof(seq_t));
        if (seq == NULL) abort();
        seqCap = newCap;
    }

    /* Generate terms until we reach the requested index n */
    while (length < n) {
        seq_t cand5 = seq[ptr5] * (seq_t)5;
        seq_t cand9 = seq[ptr9] * (seq_t)9;
        seq_t next  = cand5 < cand9 ? cand5 : cand9;

        if (cand5 == cand9) {
            ptr5++;
            ptr9++;
        } else if (cand5 < cand9) {
            ptr5++;
        } else {
            ptr9++;
        }

        seq[length++] = next;
    }

    return seq[n - 1];   /* array is 0‑based */
}
