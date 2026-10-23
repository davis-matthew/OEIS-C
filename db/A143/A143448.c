/*
 * A143448  Expansion of 1/(x^k*(1-x-2*x^(k+1))) for k=5.
 * https://oeis.org/A143448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-6) for n >= 7, with a(0)=1, a(1)=3,
 *            a(2)=5, a(3)=7, a(4)=9, a(5)=11, a(6)=13
 * Range:     u8 n <= 14, u16 n <= 31, u32 n <= 65, u64 n <= 132,
 *            u128 n <= 268, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For small n use the explicit formula */
    if (n <= 6)
        return ((seq_t)2 * (seq_t)n) + (seq_t)1;

    /* Circular buffer of the last seven terms */
    seq_t buf[7] = { (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)7,
                     (seq_t)9, (seq_t)11, (seq_t)13 };

    for (idx_t i = 7; i <= n; ++i) {
        idx_t cur = i % 7;
        idx_t prev1 = (i + 6) % 7;      /* i-1 modulo 7 */
        idx_t prev6 = (i + 1) % 7;      /* i-6 modulo 7 */
        buf[cur] = buf[prev1] + ((seq_t)2 * buf[prev6]);
    }

    return buf[n % 7];
}
