/*
 * A106264  Diagonal sums of number triangle A106262.
 * https://oeis.org/A106264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (2^(n-2*k) mod (k+2))
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;              /* floor(n/2) */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t exp = n - 2 * k;
        seq_t mod = (seq_t)(k + 2);
        seq_t base = (seq_t)2 % mod;
        seq_t pow = 1;
        idx_t e = exp;
        while (e > 0) {
            if (e & 1)
                pow = (pow * base) % mod;
            base = (base * base) % mod;
            e >>= 1;
        }
        sum += pow;
    }
    return sum;
}
