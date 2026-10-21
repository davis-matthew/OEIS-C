/*
 * A073552  Duplicate of A067275.
 * https://oeis.org/A073552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 0, n = 0 a(n) = 1, n = 1 a(n) = 7 + 60 * (10^(n-2) - 1) /
 *            9, n >= 2
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two special cases first */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Compute 10^(n-2) */
    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow10 *= (seq_t)10;

    /* Apply the formula 7 + 60 * (pow10 - 1) / 9 */
    seq_t val = (pow10 - (seq_t)1) / (seq_t)9;
    val *= (seq_t)60;
    val += (seq_t)7;

    return val;
}
