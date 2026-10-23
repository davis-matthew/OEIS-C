/*
 * A144082  Eigentriangle generated from inverse of 6th cyclotomic polynomial,
 *          row sums = n+1.
 * https://oeis.org/A144082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the linearized lower‑triangular matrix generated from *
 *            the inverse of the 6th cyclotomic polynomial. Let A010892(i) * =
 *            (1,1,0,-1,-1,0) repeated and B(k)=max(k,1). * For n ≥ 0, find
 *            the row r such that n < (r+1)(r+2)/2, set * k = n – r(r+1)/2
 *            (column index in that row). Then * a(n) = A01...
 * Range:     i8 n <= 88, i16 n <= 88, i32 n <= 88, i64 n <= 88, i128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t prev = 0;               /* number of elements in rows before r   */
    idx_t r    = 0;               /* current row index                     */
    while (n >= prev + r + 1) {
        prev += r + 1;            /* elements added by this row            */
        r++;
    }
    idx_t k = n - prev;           /* column within row r (0‑based)         */
    idx_t idx = (r - k) % 6;      /* index into A010892 periodic seq      */
    static const seq_t a010892[6] = { (seq_t)1, 1, 0, -1, -1, 0 };
    seq_t val = a010892[idx];
    if (k == 0)
        return val;               /* B(0) = 1                             */
    return val * (seq_t)k;         /* B(k) = k for k>0                     */
}
