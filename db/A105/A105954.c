/*
 * A105954  Array read by descending antidiagonals: A(n, k) = (n + 1)! * H(k,
 *          n + 1), where H(n, k) is a higher-order harmonic number, H(0, k) =
 *          1/k and H(n, k) = Sum_{j=1..k} H(n-1, j), for 0 <= k <= n.
 * https://oeis.org/A105954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = sequence A105954 (Array read by descending antidiagonals)
 * Range:     u8 n <= 18, u16 n <= 41, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is obtained from the triangular array A(r, c) with            */
/*   A(r, c) = (r + c)! * (H(r + c) - H(c - 1))  for c > 0
/*   A(r, 0) = r!                                                    */
/* The index n is mapped to the pair (r, c) by traversing n‑by‑n      */
/* antidiagonals: (0,0), (0,1),(1,0), (0,2),(1,1),(2,0), ...        */

seq_t a(idx_t n)
{
    /* Find the diagonal s (sum r+c) containing the n-th term.          */
    idx_t diag = 0;
    while (diag * (diag + 1) / 2 <= n)     /* previous cumulative count   */
        diag++;
    diag--;                                /* largest diag with prev <= n */
    idx_t prev = diag * (diag + 1) / 2;     /* terms before this diag      */
    idx_t row = n - prev;                   /* r = row index              */
    idx_t col = diag - row;                 /* c = column index           */

    /* Special case col == 0 : A(r,0) = r!            */
    if (col == 0)
    {
        seq_t fact = (seq_t)1;
        for (idx_t i = 2; i <= row; ++i)
            fact *= (seq_t)i;
        return fact;
    }

    /* General case: compute P = product_{i=col}^{row+col} i = (row+col)!/(col-1)! */
    idx_t m = row + col;   /* m = r + c = diag                         */
    seq_t P = (seq_t)1;
    for (idx_t i = col; i <= m; ++i)
        P *= (seq_t)i;

    /* A(r,c) = sum_{i=col}^{m} P / i                      */
    seq_t result = (seq_t)0;
    for (idx_t i = col; i <= m; ++i)
        result += P / (seq_t)i;

    return result;
}
