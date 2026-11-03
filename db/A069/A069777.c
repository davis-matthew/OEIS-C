/*
 * A069777  Array of q-factorial numbers n!_q, read by ascending
 *          antidiagonals.
 * https://oeis.org/A069777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = read the 2‑D array T(n,q) = ∏_{k=1..n} (q^k−1)/(q−1)
 *            along ascending anti‑diagonals, where (n,q) is ordered by sum
 *            s=n+q and within each diagonal n decreases from s to 0 (q
 *            increases from 0 to s)
 * Range:     u8 n <= 22, u16 n <= 37, u32 n <= 56, u64 n <= 92, u128 n <= 138,
 *            big any n
 * Validated: standard, pass, 71 terms through n = 1391 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Includes common headers.  */
#include <stdint.h>
#include <math.h>

/*  Return the q‑factorial number T(row,col).  */
static seq_t q_factorial(idx_t row, idx_t col)
{
    if (row == 0 || col == 0)          /* T(0,q) = T(n,0) = 1 */
        return (seq_t)1;

    if (col == 1) {                    /* T(n,1) = n!  (ordinary factorial) */
        seq_t res = 1;
        for (idx_t i = 2; i <= row; ++i)
            res *= (seq_t)i;
        return res;
    }

    /* General case: product of A_k with A_k = (q^k−1)/(q−1) and
       recurrence A_k = A_{k−1}*q + 1  (A_1 = 1).  */
    seq_t res   = 1;
    seq_t factor = 1;   /* = A_1 */
    for (idx_t k = 1; k <= row; ++k) {
        res *= factor;
        factor = factor * (seq_t)col + (seq_t)1;
    }
    return res;
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    /*  Find the diagonal index s such that s(s+1)/2 ≤ n < (s+1)(s+2)/2.  */
    idx_t s = (idx_t)((sqrtl((long double)8 * (long double)n + 1) - 1) / 2);

    /*  Number of entries before diagonal s.  */
    idx_t prev = s * (s + 1) / 2;

    /*  Position inside diagonal s.  */
    idx_t offset = n - prev;          /* 0 ≤ offset ≤ s ,  col = offset,  row = s-offset */
    idx_t row = s - offset;
    idx_t col = offset;

    return q_factorial(row, col);
}
