/*
 * A176672  a(2*n) = 1 + 6*n, a(2*n+1) = A165367(n).
 * https://oeis.org/A176672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-4) - a(n-8) (n ≥ 8) with initial values a(0)=1,
 *            a(1)=1, a(2)=7, a(3)=5, a(4)=13, a(5)=4, a(6)=19, a(7)=11
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t len = 0;
    static idx_t cap = 0;

    /* initialise cache at first call */
    if (!seq) {
        cap = 8;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        seq[0] = 1;  seq[1] = 1;  seq[2] = 7;  seq[3] = 5;
        seq[4] = 13; seq[5] = 4;  seq[6] = 19; seq[7] = 11;
        len = 8;
    }

    if (n < len)
        return seq[n];

    /* extend cache up to required index n using recurrence */
    for (idx_t i = len; i <= n; ++i) {
        seq_t next = 2 * seq[i - 4] - seq[i - 8];
        if (len >= cap) {
            cap = cap ? cap << 1 : 8;
            seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
        }
        seq[len++] = next;
    }

    return seq[n];
}
