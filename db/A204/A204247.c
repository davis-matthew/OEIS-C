/*
 * A204247  Determinant of the n-th principal submatrix of A204246.
 * https://oeis.org/A204247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (Product_{k=1..n-1} k!) * (1 - Sum_{k=1..n-1} 1/k!)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 7, i64 n <= 9, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset: a(0) = a(1) = 1 */
    if (n <= 1) return (seq_t)1;

    /* a(2) = 0, product A_2 = 1! = 1 */
    seq_t A = (seq_t)1;    /* product of factorials up to (cur-1) */
    seq_t val = (seq_t)0;  /* current a(cur)  */
    seq_t factorial = (seq_t)1;  /* (cur-1)! initially 1! */

    /* cur is the current index n in the recurrence
       a(cur+1) = a(cur) * cur! - A(cur)   */
    idx_t cur = 2;
    for (; cur < n; ++cur) {
        factorial = factorial * (seq_t)cur;        /* cur!  */
        seq_t next_val = val * factorial - A;      /* a(cur+1) */
        seq_t next_A   = A * factorial;            /* A(cur+1) */
        val = next_val;
        A   = next_A;
    }
    return val;
}
