/*
 * A211019  Triangle read by rows: T(n,k) = number of squares and rectangles
 *          of area 2^(k-1) after 2^n stages in the toothpick structure of
 *          A139250, divided by 4, n>=1, k>=1, assuming the toothpicks have
 *          length 2.
 * https://oeis.org/A211019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term when the triangle T(n,k) is flattened by
 *            rows, where T(n,1) = 2*(4^(n-2)-1)/3 for n≥2 (T(1,1)=0) T(n,2) =
 *            (10*4^(n-3)-1)/3 for n≥3 (T(2,2)=1) T(n,k) = T(n-1,k-1) for k≥3
 *            (shifts previous row) The final term a(n) is obtained by
 *            locating the row r and column c for the index n a...
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static seq_t T1(idx_t n)
{
    if (n == 1) return 0;
    idx_t exp = 2 * (n - 2);          /* 4^(n-2) = 2^(2*(n-2)) */
    seq_t p   = pow2(exp);
    seq_t res = (seq_t)2 * p - (seq_t)2;   /* 2*(4^(n-2)) - 2 */
    return res / (seq_t)3;          /* division is exact */
}

static seq_t T2(idx_t n)
{
    if (n == 2) return 1;            /* T(2,2)=1 */
    idx_t exp = 2 * (n - 3);          /* 4^(n-3) = 2^(2*(n-3)) */
    seq_t p   = pow2(exp);
    seq_t res = (seq_t)10 * p - (seq_t)1;    /* 10*4^(n-3) - 1 */
    return res / (seq_t)3;          /* division is exact */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety, though n≥1 always */

    /* locate row r: smallest r with r(r+1)/2 ≥ n */
    idx_t r   = 1;
    idx_t acc = 1;                   /* cumulative count up to row r */
    while (acc < n) {
        r++;
        acc += r;
    }

    /* column in this row */
    idx_t first_of_row = acc - r + 1;
    idx_t c = n - first_of_row + 1;

    if (c == 1) return T1(r);
    if (c == 2) return T2(r);

    /* for k≥3: T(r,k) = T(r-(k-2),2) */
    idx_t n2 = r - (c - 2);
    return T2(n2);
}
