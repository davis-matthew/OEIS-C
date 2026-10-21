/*
 * A213849  Rectangular array: (row n) = b**c, where b(h) = ceiling(h/2), c(h)
 *          = floor(n-1+h), n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is obtained by reading the infinite array
 *  T(r,c) = Σ_{h=1}^{c} ⌈h/2⌉ · ⌊(r + c – h + 1)/2⌋
 *  in antidiagonal order (row index i runs from 1 to d on diagonal d).
 *  This function finds the diagonal that contains n,
 *  converts the index to (row,column), and evaluates the sum above.
 *  The routine uses only integer arithmetic and runs in O(√n) time. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Locate the diagonal d: the smallest d with d*(d+1)/2 >= n.  */
    idx_t d = 0;
    idx_t sum = 0;                       /* sum = d*(d+1)/2 as we iterate. */
    while (sum < n) { d++; sum += d; }

    idx_t prev = sum - d;                /* sum of points in previous diagonals. */
    idx_t offset = n - prev - 1;         /* 0‑based position inside diagonal d. */
    idx_t row = offset + 1;              /* row index (1‑based).              */
    idx_t col = d - offset;              /* column index (1‑based).          */

    /* Evaluate the convolution sum for (row,col).          */
    unsigned long long acc = 0;
    for (idx_t h = 1; h <= col; ++h) {
        unsigned long long term1 = (h + 1) / 2;                  /* ceil(h/2)     */
        unsigned long long term2 = (col - h + 1 + row) / 2;      /* floor((row+col-h+1)/2) */
        acc += term1 * term2;
    }

    return (seq_t)acc;
}
