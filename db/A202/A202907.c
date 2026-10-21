/*
 * A202907  Expand 1/(1 - (3/2)*x + (2/3)*x^4 - x^5) in powers of x, then
 *          multiply coefficient of x^n by 3^floor(n/4)*2^n to get integers.
 * https://oeis.org/A202907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 211*a(n-4) + 7776*a(n-8) for n > 7 with initial values
 *            a(0)=1, a(1)=3, a(2)=9, a(3)=27, a(4)=211, a(5)=633, a(6)=1899,
 *            a(7)=5697
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _a0[8] = { (seq_t)1, (seq_t)3, (seq_t)9, (seq_t)27,
                              (seq_t)211, (seq_t)633, (seq_t)1899, (seq_t)5697 };

seq_t a(idx_t n)
{
    /* return a known seed directly for small n */
    if (n < 8)
        return _a0[(int)n];

    /* circular buffer holding the last eight terms */
    seq_t buf[8];
    for (int i = 0; i < 8; ++i)
        buf[i] = _a0[i];

    for (idx_t i = 8; i <= n; ++i) {
        idx_t  idx  = i & 7;          /* i  mod 8  */
        idx_t  idx4 = (i - 4) & 7;    /* (i-4) mod 8 */
        /* a(i) = 211*a(i-4) + 7776*a(i-8) */
        seq_t term = (seq_t)211 * buf[idx4] + (seq_t)7776 * buf[idx];
        buf[idx] = term;
    }
    return buf[n & 7];
}
