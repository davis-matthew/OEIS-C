/*
 * A206143  Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three counterclockwise and three clockwise edge
 *          increases.
 * https://oeis.org/A206143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=81, a(2)=60, a(3)=162, a(n)=3*a(n-1) for n>3
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)81;
    if (n == (idx_t)2) return (seq_t)60;
    if (n == (idx_t)3) return (seq_t)162;

    idx_t exp = n - (idx_t)3;    /* exponent for 3^(n-3) */
    seq_t pow3 = (seq_t)1;
    seq_t base = (seq_t)3;

    while (exp) {
        if (exp & (idx_t)1ULL) pow3 *= base;
        base *= base;
        exp >>= (idx_t)1ULL;
    }
    return (seq_t)162 * pow3;
}
