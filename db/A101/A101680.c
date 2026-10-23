/*
 * A101680  A modular binomial transform of 10^n.
 * https://oeis.org/A101680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum{k=0..n} (binomial(2n-k, k) mod 2) * 10^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    /* iterate over k from n down to 0, building the decimal number
       from most significant to least significant digit. */
    idx_t k = n;
    while (1)
    {
        /* m = 2*n - k */
        idx_t m = 2 * n - k;
        unsigned b = 0;
        /* binomial(2n-k, k) is odd iff every bit set in k
           occurs in m as well (Lucas theorem). */
        if (k <= m && ((k & ~m) == 0))
            b = 1;
        result = result * (seq_t)10 + b;
        if (k == 0)
            break;
        --k;
    }
    return result;
}
