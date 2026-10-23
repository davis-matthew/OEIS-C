/*
 * A144030  a(0) = 0; thereafter, a(n) = (a(n - 1) - 2)^2 - n^3.
 * https://oeis.org/A144030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 6, i128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0; a(n) = (a(n-1)-2)^2 - n^3 for n>=1 */
seq_t a(idx_t n)
{
    seq_t val = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t t = val - (seq_t)2;   /* val-2 */
        t = t * t;                  /* (val-2)^2 */
        seq_t ii = (seq_t)i;        /* n as seq_t */
        seq_t n3 = ii * ii * ii;    /* n^3 */
        val = t - n3;               /* new a(i) */
    }
    return val;
}
