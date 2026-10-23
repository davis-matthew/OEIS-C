/*
 * A102026  Number of n-bit strings that contain no more than 4 zeros and no
 *          more than 2 leading and 2 trailing zeros.
 * https://oeis.org/A102026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102026: number of n-bit strings with ≤4 zeros, 
 * ≤2 leading zeros, and ≤2 trailing zeros.
 *
 * Generating function gives the linear recurrence
 *
 *    a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5)   for n ≥ 8
 *
 * with initial values
 *
 *    a(1)= 2,  a(2)= 4,  a(3)= 7,  a(4)=13,  a(5)=25,
 *    a(6)=49,  a(7)=97.
 *
 * This implementation computes the sequence in O(n) time per call
 * using only addition and assignment, and works for any unsigned
 * integer type supplied as seq_t (C integer or arbitrary‑precision). */

#include <stdint.h>   /* for uint64_t (idx_t) */

seq_t a(idx_t n)
{
    /* Handle the initial segment explicitly */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)13;
    if (n == 5) return (seq_t)25;
    if (n == 6) return (seq_t)49;
    if (n == 7) return (seq_t)97;

    /* For n ≥ 8 use the recurrence.
       We keep the last five values in a sliding window. */
    seq_t v3 = (seq_t)7;   /* a(3) */
    seq_t v4 = (seq_t)13;  /* a(4) */
    seq_t v5 = (seq_t)25;  /* a(5) */
    seq_t v6 = (seq_t)49;  /* a(6) */
    seq_t v7 = (seq_t)97;  /* a(7) */
    seq_t next;

    for (idx_t i = 8; i <= n; ++i) {
        next = v3 + v4 + v5 + v6 + v7;   /* a(i) */
        /* slide the window: drop v3, shift others, store new value */
        v3 = v4;
        v4 = v5;
        v5 = v6;
        v6 = v7;
        v7 = next;
    }
    /* after loop, v7 holds a(n) for n ≥ 8 */
    return v7;
}
