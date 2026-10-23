/*
 * A144113  Weight array W={w(i,j)} of the natural number array A038722.
 * https://oeis.org/A144113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = { j if i == 1 { i-1 if j == 1 { 1 otherwise, where (i,j)
 *            are obtained by scanning the weight array W of A038722 in
 *            antidiagonal order: let d be the smallest integer with d*(d-1)/2
 *            >= n (d ≥ 2). let i = n - (d-2)*(d-1)/2 (1‑based index within
 *            the d‑th diagonal), let j = d - i
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* find diagonal d such that d*(d-1)/2 >= n */
    idx_t d = 2;
    for (;; ++d) {
        unsigned __int128 t = (unsigned __int128)d * (d - 1) / 2;
        if (t >= n)
            break;
    }

    /* number of terms preceding diagonal d */
    unsigned __int128 prev128 = (unsigned __int128)(d - 2) * (d - 1) / 2;
    idx_t prev = (idx_t)prev128;

    /* position inside diagonal */
    idx_t i = n - prev;          /* 1‑based row index within the diagonal */
    idx_t j = d - i;             /* corresponding column index */

    /* compute weight according to the definition of W */
    if (i == 1)
        return (seq_t)j;              /* w(1,j) = j */
    if (j == 1)
        return (seq_t)(i - 1);        /* w(i,1) = i-1 for i>1 */
    return (seq_t)1;                 /* w(i,j)=1 otherwise */
}
