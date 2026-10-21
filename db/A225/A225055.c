/*
 * A225055  Irregular triangle which lists the three positions of 2*n-1 in
 *          A060819 in row n.
 * https://oeis.org/A225055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 2*r-1 if (n-1)%3==0; 4*r-2 if (n-1)%3==1; 8*r-4 if
 *            (n-1)%3==2; where r = (n-1)/3 + 1. This reproduces the irregular
 *            triangle of A225055
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine row (r) and column (k) in the 3‑column triangle. */
    idx_t r = (n - 1) / 3 + 1;          /* 1‑based row number */
    idx_t k = (n - 1) % 3;              /* 0‑based column index: 0,1,2 */

    idx_t val;                          /* 64‑bit intermediate value */
    switch (k) {
        case 0:  /* first column: T(r,1) = 2*r - 1 */
            val = 2 * r - 1;
            break;
        case 1:  /* second column: T(r,2) = 4*r - 2 */
            val = 4 * r - 2;
            break;
        default:/* third column: T(r,3) = 8*r - 4 */
            val = 8 * r - 4;
            break;
    }
    return (seq_t)val;                  /* cast to seq_t, exact if it fits */
}
