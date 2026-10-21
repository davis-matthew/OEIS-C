/*
 * A204162  Symmetric matrix based on f(i,j) = (floor((i+1)/2) if i=j and = 1
 *          otherwise), by antidiagonals.
 * https://oeis.org/A204162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 except when the n‑th element in the antidiagonal
 *            traversal of the infinite symmetric matrix M(i,j) is on the main
 *            diagonal; then a(n)=floor((i+1)/2)
 * Range:     u8 n <= 9131, u16 n <= 9131, u32 n <= 9131, u64 n <= 9131,
 *            u128 n <= 9131, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find antidiagonal S such that S(S-1)/2 >= n. */
    idx_t low = 1, high = 1;
    while (((__uint128_t)(high-1)*high)/2 < n) high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t cum = (__uint128_t)(mid-1)*mid/2;
        if (cum >= n) high = mid;
        else low = mid + 1;
    }
    idx_t S = low;                          /* antidiagonal index */
    __uint128_t prev = (__uint128_t)(S-2)*(S-1)/2; /* cumulative count before this diagonal */
    idx_t offset = (idx_t)(n - prev);          /* 1‑based position within the diagonal */

    if ((S & 1U) == 0 && offset == S/2) {     /* diagonal element */
        seq_t val = (seq_t)(offset + 1) / 2;   /* floor((i+1)/2) with i=offset */
        return val;
    }
    return (seq_t)1;
}
