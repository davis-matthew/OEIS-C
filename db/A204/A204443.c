/*
 * A204443  Symmetric matrix: f(i,j)=floor[(i+j+3)/4]-floor[(i+j)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 if the minimal integer s with (s-1)*s/2 ≥ n is
 *            divisible by 4; otherwise a(n) = 1. The index n is read along
 *            anti‑diagonals of the infinite matrix f(i,j) = floor[(i+j+3)/4]
 *            - floor[(i+j)/4]
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uintptr_t, if needed; not strictly required */

seq_t a(idx_t n)
{
    /* n ≥ 1 according to the OEIS offset; we still handle 0 safely. */
    if (n == 0) return 0;

    /* Find the smallest s such that T(s) = (s-1)*s/2 ≥ n.
       We accumulate the triangular numbers incrementally.  Use
       128‑bit arithmetic for the accumulator to avoid overflow. */
    idx_t s = 1;
    unsigned __int128 acc = 0;

    while (acc < n) {
        ++s;                     /* move to the next diagonal (sum = s) */
        acc += (unsigned __int128)(s - 1);   /* add length of this diagonal */
    }

    /* The value depends only on s mod 4. */
    return (seq_t)(s % 4 == 0 ? 0 : 1);
}
