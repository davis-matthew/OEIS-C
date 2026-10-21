/*
 * A079947  Partial sums of A030300.
 * https://oeis.org/A079947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( n + 1 + 2/3*(4^e4 - 1) + (-1)^e2 * ( n - 1 - 2*(4^e4 -
 *            1) ) ) / 2, where e2 = floor(log2(n)), e4 = floor(log4(n))
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute e2 = floor(log2(n)) */
    int e2 = 0;
    uint64_t tt = n;
    while (tt > 1) {
        tt >>= 1;
        e2++;
    }
    /* compute e4 = floor(log4(n)) = e2 / 2 */
    int e4 = e2 >> 1;

    /* 4^e4 as seq_t */
    seq_t pow4 = (seq_t)(1ULL << (2 * e4));   /* safe since 2*e4 <= 62 for 64-bit n */

    seq_t pow4m1 = pow4 - 1;

    /* term1 = n + 1 + 2/3 * (4^e4 - 1) */
    seq_t term1 = (seq_t)n + 1 + ((pow4m1 / 3) * 2);

    /* inner part = n - 1 - 2*(4^e4 - 1) */
    seq_t inner = (seq_t)n - 1 - 2 * pow4m1;

    /* final value depends on parity of e2 */
    if (e2 & 1) {          /* (-1)^e2 = -1 */
        return (term1 - inner) / 2;
    } else {                /* (-1)^e2 = +1 */
        return (term1 + inner) / 2;
    }
}
