/*
 * A008669  Molien series for 4-dimensional complex reflection group of order
 *          7680 (in powers of x^4).
 * https://oeis.org/A008669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((n+3)*(2n+9)*(n+9)/360)
 * Range:     u8 none, u16 n <= 26, u32 n <= 1413, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partition of n into parts 1, 2, 3 and 5 (OEIS A008669) */

/* Computes a(n) for any nonnegative idx_t n.  The calculation uses only
 * integer arithmetic and works for any seq_t that can hold the result. */
seq_t a(idx_t n)
{
    /* Cast n to seq_t before any arithmetic to avoid overflow on the
     * index value.  All multiplications are performed in seq_t.
     */
    seq_t nn  = (seq_t)n;
    seq_t f1  = nn + (seq_t)3;       /* n + 3   */
    seq_t f2  = (nn << 1) + (seq_t)9;/* 2n + 9  */
    seq_t f3  = nn + (seq_t)9;       /* n + 9   */

    /* num = (n+3)*(2n+9)*(n+9) */
    seq_t num = f1 * f2 * f3;

    /* Divide by 360 with rounding to nearest integer:
     * rounding(x/y) = floor((x + y/2)/y) for positive integers.
     */
    return (num + (seq_t)180) / (seq_t)360;
}
