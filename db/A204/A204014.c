/*
 * A204014  Symmetric matrix based by antidiagonals, based on f(i,j)=min{1+(j
 *          mod i), 1+( i mod j)}.
 * https://oeis.org/A204014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A204014: a(n) is the (i,j)-entry of the symmetric matrix
   M defined by f(i,j)=min{1+(j mod i), 1+(i mod j)},
   enumerated by antidiagonals (i+j = constant), reading each
   diagonal with i increasing. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 1.., find diagonal s such that
       T_{s-1} = (s-1)*s/2  >= n  (triangular number). */
    idx_t s      = 2;          /* start with diagonal 2 (contains (1,1)) */
    idx_t count  = 1;          /* cumulative entries up to current s */

    while (count < n) {
        ++s;                   /* move to next diagonal */
        count += s - 1;        /* add entries in this diagonal */
    }

    idx_t prev   = count - (s - 1);   /* entries before diagonal s */
    idx_t i      = n - prev;          /* i-th element in diagonal s (1‑based) */
    idx_t j      = s - i;             /* j = s - i */

    /* Compute f(i,j) = min{1+(j mod i), 1+(i mod j)}.  Since i,j
       are small (<= sqrt(2n)), 64‑bit arithmetic suffices. */
    idx_t r1 = 1 + (j % i);
    idx_t r2 = 1 + (i % j);

    seq_t v1 = (seq_t)r1;
    seq_t v2 = (seq_t)r2;
    return v1 < v2 ? v1 : v2;
}
