/*
 * A190570  Number of n-step one-sided prudent walks, avoiding exactly two
 *          consecutive west steps and two consecutive east steps.
 * https://oeis.org/A190570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) + 2*a(n-4), 
 * with a(1)=1, a(2)=3, a(3)=5, a(4)=13.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* return 0 for indices below the offset (not part of the sequence) */
    if (n < 1) return (seq_t)0;

    /* handle the first four terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)13;

    /* iterate the recurrence from 5 up to n */
    seq_t a_nm4 = (seq_t)1; /* a(n-4) */
    seq_t a_nm3 = (seq_t)3; /* a(n-3) */
    seq_t a_nm2 = (seq_t)5; /* a(n-2) */
    seq_t a_nm1 = (seq_t)13;/* a(n-1) */
    seq_t a_n = (seq_t)0;

    for (idx_t k = 5; k <= n; ++k) {
        a_n = (seq_t)2 * a_nm1 + a_nm2 - (seq_t)2 * a_nm3 + (seq_t)2 * a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
