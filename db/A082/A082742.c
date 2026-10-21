/*
 * A082742  Indices of occurrences of 2 in A004738.
 * https://oeis.org/A082742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2m-1) = m(m+1), a(2m) = m(m+1)+2
 * Range:     u8 n <= 30, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = ceil(n/2) = floor(n/2) + n%2 */
    seq_t m = (seq_t)(n / 2 + n % 2);
    seq_t val = m * (m + 1);   /* = m(m+1) */
    if (n & 1)                 /* odd n → a(2m-1) */
        return val;
    else                       /* even n → a(2m) = val + 2 */
        return val + 2;
}
