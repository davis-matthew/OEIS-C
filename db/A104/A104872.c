/*
 * A104872  Diagonal sums of A004248.
 * https://oeis.org/A104872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} k^(n-2*k)
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 23, u64 n <= 36, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t e = n - 2 * k;
        seq_t term;
        if (k == 0) {
            term = (e == 0) ? (seq_t)1 : (seq_t)0;
        } else {
            seq_t res = 1;
            seq_t b = (seq_t)k;
            idx_t exp = e;
            while (exp > 0) {
                if (exp & 1) res = res * b;
                b = b * b;
                exp >>= 1;
            }
            term = res;
        }
        sum += term;
    }
    return sum;
}
