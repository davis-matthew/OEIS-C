/*
 * A211670  Number of iterations (...(f_4(f_3(f_2(n))))...) such that the
 *          result is < 2, where f_j(x) := x^(1/j).
 * https://oeis.org/A211670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A211670: Number of consecutive root iterations f_j(x)=x^(1/j) needed to make
   the result less than 2, starting with f_2.  Equivalently,
   a(n) = min { k ≥ 2 : k! > floor(log₂(n)) } - 1,
   with a(1) = 0. */
#include <stdint.h>

/* Compute floor(log₂(v)) for an unsigned 64‑bit integer. */
static inline idx_t floor_log2_idx(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* For the initial term 1 the result is already < 2, so zero iterations. */
    if (n < 2)
        return 0;

    /* t = ⌊log₂(n)⌋  */
    idx_t t = floor_log2_idx(n);

    /* Find the smallest j ≥ 2 with j! > t.  Since t ≤ 63 for 64‑bit n,
       j will be at most 7.  We use seq_t for factorial to avoid overflow
       for larger seq_t types. */
    seq_t fact = 2;   /* 2!   */
    idx_t j = 2;
    while (fact <= (seq_t)t) {
        ++j;
        fact *= (seq_t)j;   /* j stays small; result fits in seq_t. */
    }

    /* a(n) = j - 1 */
    return (seq_t)(j - 1);
}
