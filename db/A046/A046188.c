/*
 * A046188  Indices of octagonal numbers which are also pentagonal.
 * https://oeis.org/A046188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A046188: Indices of octagonal numbers which are also pentagonal.
 * Recurrence (valid for n ≥ 6):
 *     a(1) = 1,  a(2) = 8,  a(3) = 725,
 *     a(4) = 8844, a(5) = 836265,
 *     a(n) = 1154 * a(n-2) - a(n-4) - 384
 */

#pragma once
#include <stdint.h> /* for uint64_t if needed, but seq_t/idx_t are defined elsewhere */

seq_t a(idx_t n)
{
    /* sequence is indexed from 1; return 0 for 0 or negative indices */
    if (n < 1) return (seq_t)0;

    /* first five terms */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)725;
    if (n == 4) return (seq_t)8844;
    if (n == 5) return (seq_t)836265;

    /* use a sliding window of length 5:
     *  win[0]=a(n-4), win[1]=a(n-3), win[2]=a(n-2),
     *  win[3]=a(n-1), win[4]=a(n)   (the term just computed)
     */
    seq_t win[5];
    win[0] = (seq_t)1;      /* a(1)   */
    win[1] = (seq_t)8;      /* a(2)   */
    win[2] = (seq_t)725;    /* a(3)   */
    win[3] = (seq_t)8844;   /* a(4)   */
    win[4] = (seq_t)836265; /* a(5)   */

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = 1154 * a(i-2) - a(i-4) - 384   */
        seq_t next = (seq_t)1154 * win[3] - win[1] - (seq_t)384;

        /* shift window: drop a(i-4), move others left, append next */
        win[0] = win[1];
        win[1] = win[2];
        win[2] = win[3];
        win[3] = win[4];
        win[4] = next;
    }

    return win[4]; /* a(n) */
}
