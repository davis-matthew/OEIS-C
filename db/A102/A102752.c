/*
 * A102752  Array read by antidiagonals: T(n, k) = ((n+2)^k-(n-1)^k)/3.
 * https://oeis.org/A102752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read by antidiagonals from T(i,j)=((i+2)^j-(i-1)^j)/3
 * Range:     u8 n <= 24, u16 n <= 61, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   diag_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/diag_of.h"

static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1U)
            res *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Convert linear index to (row, col) in the array */
    idx_t s = diag_of(n);
    unsigned __int128 cumulative = (unsigned __int128)s * (s + 1);
    cumulative >>= 1;
    idx_t pos = n - (idx_t)cumulative;   /* position inside the diagonal */
    idx_t row = s - pos;                 /* row index i */
    idx_t col = pos;                     /* column index j */

    if (row == 0) {
        /* T(0,j) = ((2)^j - (-1)^j)/3 */
        seq_t p2 = pow_seq((seq_t)2, col);
        if ((col & 1U) == 0)   /* (-1)^j = 1  */
            return (p2 - (seq_t)1) / (seq_t)3;
        else                   /* (-1)^j = -1 */
            return (p2 + (seq_t)1) / (seq_t)3;
    } else {
        /* T(i,j) = ((i+2)^j - (i-1)^j)/3,  i >= 1 */
        seq_t a1 = pow_seq((seq_t)(row + 2), col);
        seq_t a2 = pow_seq((seq_t)(row - 1), col);
        return (a1 - a2) / (seq_t)3;
    }
}
