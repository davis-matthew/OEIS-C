/*
 * A122958  a(0)=1, a(n) = 2 - 2^(n-1) for n>0.
 * https://oeis.org/A122958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(n) = 2 - 2^(n-1) for n > 0 The recurrence a(n+1) =
 *            2*a(n) - 2 for n ≥ 0 is used for efficiency
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)      return (seq_t)1;   /* a(0) */
    seq_t val = (seq_t)1;                /* a(1) */
    for (idx_t i = 1; i < n; ++i)        /* compute a(2) … a(n) */
    {
        val = val * (seq_t)2 - (seq_t)2;
    }
    return val;
}
