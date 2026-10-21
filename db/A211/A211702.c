/*
 * A211702  Rectangular array: R(n,k)=[n/F(1)]+[n/F(2)]+...+[n/F(k)], where [
 *          ]=floor and F=A000045 (Fibonacci numbers), by antidiagonals.
 * https://oeis.org/A211702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ∑_{k=1}^{j} ⌊ i / F_k ⌋ where (i , j) is obtained from n
 *            by the antidiagonal enumeration of the rectangular array R(i ,
 *            j)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*---------------------------------------------------------------*/
/* needed standard headers (seq_t and idx_t are supplied by the
   build system; no other types are defined here).                */
/*---------------------------------------------------------------*/
#include <stddef.h>
#include <stdbool.h>

/*---------------------------------------------------------------*/
/* compute the term a(n) for n ≥ 1                               */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* 1. find the diagonal number s such that
       triangular(s-1) < n ≤ triangular(s)  */
    idx_t left  = 1;
    idx_t right = n + 1;           /* n ≥ 1 ⇒ s ≤ n+1 is safe */

    while (left < right) {
        idx_t mid = (left + right) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            right = mid;
        else
            left = mid + 1;
    }
    idx_t s = left;                /* sum of row+col for the diagonal */

    /* 2. compute the number of entries up to the previous diagonal */
    unsigned __int128 prev_tri =
        (unsigned __int128)(s - 1) * s / 2;
    idx_t prev = (idx_t)prev_tri;  /* safe cast: prev ≤ n */

    /* 3. position inside the current diagonal (0‑based) */
    idx_t pos  = n - prev - 1;      /* pos ∈ [0 , s-1] */

    /* 4. coordinates (row, column) in the array */
    idx_t row = s - pos;          /* row ≥ 1 */
    idx_t col = pos + 1;          /* col ≥ 1 */

    /* 5. compute R(row, col) = Σ_{k=1}^{col} floor(row / F_k) */
    seq_t sum = 0;
    seq_t f_prev = 1;   /* F_1 */
    seq_t f_curr = 1;   /* F_2 */
    seq_t f_k;

    for (idx_t k = 1; k <= col; ++k) {
        if (k == 1)
            f_k = 1;            /* F_1 */
        else if (k == 2)
            f_k = 1;            /* F_2 */
        else {
            f_k = f_prev + f_curr;
            f_prev = f_curr;
            f_curr = f_k;
        }

        /* floor(row / F_k) */
        sum += ((seq_t)row) / f_k;
    }

    return sum;
}
