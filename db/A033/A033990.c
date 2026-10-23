/*
 * A033990  Write 0,1,2,... in a clockwise spiral on a square lattice, writing
 *          each digit at a separate lattice point, starting with 0 at the
 *          origin and 1 at x=0, y=-1; sequence gives the numbers on the
 *          negative y-axis.
 * https://oeis.org/A033990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = digit of the clockwise spiral at coordinate (0,-n)
 * Range:     u8 n <= 13, u16 n <= 288, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation computes the position of (0,-n) in the spiral path
   and returns the corresponding decimal digit written on that lattice point. */

#include <stdint.h>

static inline idx_t pow10_idx(int exp) {
    idx_t p = 1;
    for (int i = 0; i < exp; ++i) p *= 10;
    return p;
}

static inline seq_t pow10_seq(int exp) {
    seq_t p = 1;
    for (int i = 0; i < exp; ++i) p *= 10;
    return p;
}

/* Return the decimal digit at the given zero‑based digit position in the spiral. */
static seq_t digit_at_index(idx_t pos) {
    if (pos < 10) return (seq_t)pos;           /* 0–9 appear first. */

    idx_t rem = pos - 10;                      /* subtract first 10 digits. */
    int d = 2;                                 /* current digit length (2). */

    while (1) {
        idx_t nums   = 9 * pow10_idx(d - 1);    /* count of numbers with d digits. */
        idx_t block  = nums * d;               /* digits contributed by this block. */
        if (rem < block) break;
        rem -= block;
        ++d;
    }

    idx_t number_offset = rem / d;              /* offset within d‑digit numbers. */
    int  digit_index    = (int)(rem % d);       /* 0 = most significant digit. */

    seq_t number = pow10_seq(d - 1) + (seq_t)number_offset;
    seq_t divisor = pow10_seq(d - 1 - digit_index);
    return (seq_t)((number / divisor) % 10);
}

/* Sequence A033990: digit on the negative y‑axis in the clockwise spiral. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                      /* a(0) = 0 */
    /* index of the lattice point (0,-n) in the spiral path: */
    idx_t step = 1 + 4 * (n - 1) * n + (n - 1); /* S_start(n) + (n-1)  */
    return digit_at_index(step);
}
