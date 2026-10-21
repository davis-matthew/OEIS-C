/*
 * A208387  Number of nX3 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=5, a(2)=117, a(n)=26*a(n-1) for n>2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)5;
    if (n == 2)
        return (seq_t)117;

    idx_t e = n - 2;
    seq_t base = (seq_t)26;
    seq_t result = (seq_t)1;

    while (e)
    {
        if (e & 1)
            result *= base;        /* multiply by base if lowest bit is 1 */
        base *= base;              /* square the base for next bit */
        e >>= 1;                   /* shift right to process next bit */
    }

    result *= (seq_t)117;          /* a(n)=117*26^(n-2) */
    return result;
}
