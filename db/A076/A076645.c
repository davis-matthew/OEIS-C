/*
 * A076645  Irregular triangle read by rows of ways to write 0 as 1 +- 2 +- 3
 *          +- 4 +- ... +- k for some k, where signs alternate except that
 *          there is one instance of two consecutive positive terms.
 * https://oeis.org/A076645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A076645 – Irregular triangle of sums 0 with two consecutive positive terms.
 *  Each row corresponds to an odd k=4m-1.  For that k the signs alternate
 *  +/- except for one pair of consecutive + at positions (k-1)/2 and (k+1)/2.
 *  The value at column i (0‑based) is sign(k,i)*(i+1).
 *  a(n) is obtained by determining the row r such that
 *      2r² + r ≥ n      and 2(r‑1)² + (r‑1) < n,
 *  then the position within the row and the sign as described above.
 */

seq_t a(idx_t n)
{
    if (n == 0)        /* safety for out‑of‑bounds calls */
        return (seq_t)0;

    /* Find the row r (1-based) that contains the nth term.
       Cumulative number of terms up to row r is 2r² + r.          */
    idx_t r = 1;
    while (((2ULL * r * r) + r) < n)
        ++r;

    /* Number of terms before this row */
    idx_t pr = r - 1;
    idx_t prev = (2ULL * pr * pr) + pr;

    /* Size of this row and position inside it (0‑based) */
    idx_t k     = 4 * r - 1;                /* odd k = 4m-1 */
    idx_t pos   = n - prev - 1;             /* 0 … k-1 */

    /* Index of the first of the two consecutive positive terms
     * in 0‑based numbering: p0 = (k-3)/2   (valid for k≥3)   */
    idx_t p0 = (k - 3) / 2;

    /* Determine the sign for this position.  */
    seq_t sign;
    if (pos < p0) {                        /* alternating starting with + */
        sign = ((pos & 1) == 0) ? 1 : -1;
    } else if (pos == p0 || pos == p0 + 1) {
        sign = 1;                          /* the two consecutive + */
    } else {                               /* after the double +, start with - */
        idx_t diff = pos - (p0 + 2);
        sign = ((diff & 1) == 0) ? -1 : 1;
    }

    /* Value of the term: sign * (pos + 1)   */
    return sign * ((seq_t)(pos + 1));
}
