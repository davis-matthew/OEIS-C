/*
 * A079809  Consider a triangle in which the 2n-th row contains first 2n
 *          positive integers in increasing order and the (2n+1)-st row
 *          contains first 2n+1 positive integers in decreasing order;
 *          sequence contains concatenation of numbers read upward at a
 *          45-degree angle.
 * https://oeis.org/A079809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = concatenation of values on the diagonal starting at (row
 *            n, column 1) and moving upward‑right. The value at position
 *            (r,c) is c if r is even, otherwise r‑c+1. Thus a(n) =
 *            concat_{j=0}^{⌊(n-1)/2⌋} v(n‑j,1+j) where v(r,c) = c (r even)
 *            else r‑c+1 (r odd)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: number of decimal digits of a positive seq_t value. */
static int digit_count(seq_t x)
{
    int d = 0;
    seq_t t = x;
    do
    {
        ++d;
        t /= (seq_t)10;
    } while (t);
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for this sequence, safe default */

    seq_t result = 0;
    idx_t max_j = (n - 1) / 2;           /* number of elements on the diagonal */
    for (idx_t j = 0; j <= max_j; ++j)
    {
        idx_t r = n - j;                 /* current row        */
        idx_t c = 1 + j;                 /* current column     */
        seq_t val = (r & 1u)            /* r even? */
                      ? (seq_t)(r - c + 1)
                      : (seq_t)c;

        int d = digit_count(val);       /* digits of val      */
        seq_t tenpow = 1;
        for (int k = 0; k < d; ++k)
            tenpow *= (seq_t)10;        /* 10^d */

        result = result * tenpow + val;
    }
    return result;
}
