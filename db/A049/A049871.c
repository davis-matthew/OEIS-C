/*
 * A049871  a(n) = Sum{a(k): k=0,1,2,...,n-4,n-2,n-1}; a(n-3) is not a
 *          summand; initial terms are 1,3,3.
 * https://oeis.org/A049871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + a(n-4), a(0)=1, a(1)=3, a(2)=3,
 *            a(3)=6
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    if (seq == NULL) {
        /* reserve space for the first 4 terms */
        cap = 4;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (seq == NULL) abort();
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)3;
        seq[2] = (seq_t)3;
        seq[3] = (seq_t)6;
        len = 4;
    }
    if ((idx_t)len > n) {
        return seq[(size_t)n];
    }
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i >= 4) {
            seq_t val = seq[i - 1];
            val += val;                /* 2*a(i-1) */
            val -= seq[i - 3];          /* -a(i-3) */
            val += seq[i - 4];          /* +a(i-4) */
            if (i >= cap) {
                size_t new_cap = cap * 2;
                if (new_cap <= i) new_cap = i + 1;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                seq = tmp;
                cap = new_cap;
            }
            seq[i] = val;
        }
    }
    len = (size_t)n + 1;
    return seq[(size_t)n];
}
