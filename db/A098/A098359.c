/*
 * A098359  Multiplication table of the square numbers read by antidiagonals.
 * https://oeis.org/A098359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A098359: Multiplication table of the square numbers read by antidiagonals.
 * Formula: For each n (n ≥ 1) let d be the smallest integer with d(d+1)/2 ≥ n,
 *          and let p = n – d(d-1)/2.  Then a(n) = (p·p)·((d+1-p)·(d+1-p)).
 * This follows from the mapping of index n to coordinates (p, d+1-p) in the
 * infinite table A(i,j) = i²·j² read along antidiagonals.
 */

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is the first term. */
    if (n == 0)
        return (seq_t)0;   /* undefined but return 0 for safety */

    idx_t d   = 0;        /* current diagonal index (1-based)   */
    idx_t sum = 0;        /* cumulative number of terms up to this diagonal */

    /* Find d such that sum + d ≥ n. */
    while (sum < n) {
        ++d;
        sum += d;
    }

    idx_t prev = sum - d;       /* terms before the current diagonal   */
    idx_t pos  = n - prev;      /* position inside the current diagonal (1 ≤ pos ≤ d) */

    idx_t i = pos;              /* row index                           */
    idx_t j = d + 1 - pos;      /* column index                        */

    seq_t ii = (seq_t)i * i;    /* i²                                  */
    seq_t jj = (seq_t)j * j;    /* j²                                  */

    return ii * jj;             /* i²·j²                               */
}
