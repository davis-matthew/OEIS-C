/*
 * A140962  Negative values of the inverse binomial transform of A045883.
 * https://oeis.org/A140962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 18, i32 n <= 34, i64 n <= 66, i128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = 3*a(n-2) - 2*a(n-3)
 * Initial values: a(0)=0, a(1)=-1, a(2)=-1
 * Computes a(n) for any n ≥ 0 using an O(n) recurrence without extra storage.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)(-1);
    if (n == 2) return (seq_t)(-1);

    seq_t prev[3];
    prev[0] = (seq_t)0;     /* a(0)   */
    prev[1] = (seq_t)(-1);  /* a(1)   */
    prev[2] = (seq_t)(-1);  /* a(2)   */

    seq_t result = 0;
    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 3*a(i-2) - 2*a(i-3) */
        seq_t val = 3 * prev[(i - 2) % 3] - 2 * prev[(i - 3) % 3];
        prev[i % 3] = val;    /* store for future iterations */
        result = val;
    }
    return result;
}
