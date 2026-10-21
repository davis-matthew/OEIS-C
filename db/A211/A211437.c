/*
 * A211437  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w*x*y=n.
 * https://oeis.org/A211437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * ∏_{p^e∥n} (e+2 choose 2), a(0)=1
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t res = 1;
    idx_t tmp = n;
    for (idx_t i = 2; i <= tmp / i; ++i) {
        if (tmp % i == 0) {
            idx_t e = 0;
            while (tmp % i == 0) {
                tmp /= i;
                ++e;
            }
            seq_t comb = (seq_t)(e + 2) * (seq_t)(e + 1) / (seq_t)2;
            res *= comb;
        }
    }
    if (tmp > 1) {
        /* remaining prime factor (exponent = 1) */
        res *= (seq_t)3;
    }
    res *= (seq_t)4;
    return res;
}
