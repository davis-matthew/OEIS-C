/*
 * A134869  Row sums of triangle A134868.
 * https://oeis.org/A134869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 360, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1) = 1
 *          a(n) = n*(n+1)/2 + 1   for n > 1
 */
seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    seq_t result = nn * (nn + (seq_t)1);
    result /= (seq_t)2;
    result += (seq_t)1;
    return result;
}
