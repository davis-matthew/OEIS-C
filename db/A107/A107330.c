/*
 * A107330  a(n) = 4*a(n-1)-a(n-2)-3*a(n-3)+a(n-4), n>5.
 * https://oeis.org/A107330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 3*a(n-3) + a(n-4), n>=6 with a(0)=3,
 *            a(1)=3, a(2)=12, a(3)=41, a(4)=148, a(5)=517
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static idx_t cap = 0;
    static idx_t size = 0;

    if (seq == NULL) {
        cap = 64;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        seq[0] = (seq_t)3;
        seq[1] = (seq_t)3;
        seq[2] = (seq_t)12;
        seq[3] = (seq_t)41;
        seq[4] = (seq_t)148;
        seq[5] = (seq_t)517;
        size = 6;
    }

    if (n < size) return seq[n];
    while (size <= n) {
        if (size == cap) {
            cap <<= 1;
            seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
        }
        seq_t val = (seq_t)4 * seq[size-1]
                    - seq[size-2]
                    - (seq_t)3 * seq[size-3]
                    + seq[size-4];
        seq[size] = val;
        size++;
    }
    return seq[n];
}
