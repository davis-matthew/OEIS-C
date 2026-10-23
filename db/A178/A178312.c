/*
 * A178312  a(n) = n * T(ceiling(n/2)), where T are the triangular numbers,
 *          A000217.
 * https://oeis.org/A178312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * CEIL(n/2) * (CEIL(n/2)+1) / 2 (triangular number of
 *            ceil(n/2) multiplied by n)
 * Range:     u8 n <= 12, u16 n <= 79, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _A178312_H_
#define _A178312_H_

seq_t a(idx_t n) {
    /* ceil(n/2) = (n+1)/2 for unsigned integers */
    idx_t m = (n + 1) >> 1;  /* faster than division for unsigned types */
    seq_t ms = (seq_t)m;
    seq_t t   = ms * (ms + 1) / 2;      /* T(m) = m*(m+1)/2 */
    return (seq_t)n * t;                 /* n * T(m) */
}

#endif /* _A178312_H_ */
