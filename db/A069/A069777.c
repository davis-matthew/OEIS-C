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
 * Helpers:   q_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Includes common headers.  */
#include <stdint.h>
#include <math.h>
#include "../utils/q_factorial.h"

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
