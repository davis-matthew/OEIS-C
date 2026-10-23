/*
 * A163330  Square array A, where entry A(y,x) has the ternary digits of y
 *          interleaved with the ternary digits of x, converted back to
 *          decimal. Listed by antidiagonals: A(0,0), A(0,1), A(1,0), A(0,2),
 *          A(1,1), A(2,0), ...
 * https://oeis.org/A163330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base, look
 * Formula:   a(n) = interleave ternary digits of y and x where (y,x) is the
 *            n‑th pair in antidiagonal order of the matrix A(y,x). The pair
 *            (y,x) is obtained by decomposing n into the triangular number of
 *            the preceding diagonals
 * Range:     u8 n <= 77, u16 n <= 3320, u32 n <= 3320, u64 n <= 3320,
 *            u128 n <= 3320, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t used in idx_t */

seq_t a(idx_t n)
{
    /* Determine the antidiagonal number k (0‑based) and
       the triangular sum of all complete prior diagonals.
       After the loop:  sum = k(k+1)/2   and   k is the index of
       the current diagonal. */
    idx_t sum = 0;
    idx_t k   = 0;
    while (sum + k + 1 <= n) {
        sum += k + 1;
        ++k;
    }

    /* Coordinates on the k‑th diagonal:
       y runs from 0 to k, x = k – y. */
    idx_t y = n - sum;
    idx_t x = k - y;

    /* Interleave the ternary digits of y and x. */
    seq_t yi = (seq_t)y;
    seq_t xi = (seq_t)x;
    seq_t res   = 0;
    seq_t mult  = 1;          /* 3^position of the next digit */

    while (yi > 0 || xi > 0) {
        seq_t ydig = yi % (seq_t)3;
        seq_t xdig = xi % (seq_t)3;

        res += ydig * mult;
        mult *= (seq_t)3;
        res += xdig * mult;
        mult *= (seq_t)3;

        yi /= (seq_t)3;
        xi /= (seq_t)3;
    }

    return res;
}
