/*
 * A107242  Sum of squares of tetranacci numbers (A001630).
 * https://oeis.org/A107242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = sum_{k=0..n} F4(k)^2
 * where F4 is the tetranacci sequence:
 *   F4(0)=0, F4(1)=0, F4(2)=1, F4(3)=2
 *   F4(k)=F4(k-1)+F4(k-2)+F4(k-3)+F4(k-4)  for k≥4
 * The function computes this sum using only the seq_t type.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle small indices manually */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)5;

    /* tetranacci initial values */
    seq_t t0 = (seq_t)0; /* F4(0) */
    seq_t t1 = (seq_t)0; /* F4(1) */
    seq_t t2 = (seq_t)1; /* F4(2) */
    seq_t t3 = (seq_t)2; /* F4(3) */

    /* running sum of squares up to index 3 */
    seq_t sum = t0 * t0 + t1 * t1 + t2 * t2 + t3 * t3;

    /* iterate from index 4 up to n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t t4 = t0 + t1 + t2 + t3;   /* next tetranacci */
        sum += t4 * t4;                 /* add its square */
        /* shift the window */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
    }

    return sum;
}
