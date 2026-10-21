/*
 * A086951  a(n) = n*3^n*(4*n - 1)/9.
 * https://oeis.org/A086951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*3^n*(4*n - 1)/9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3(idx_t n)
{
    seq_t res = 1;
    seq_t base = 3;
    while (n != 0) {
        if (n & 1) {
            res *= base;
        }
        base *= base;
        n >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t n_seq = (seq_t)n;
    seq_t temp = n_seq * ((n_seq << 2) - (seq_t)1); /* n*(4*n-1) */
    temp *= pow3(n);                                 /* multiply by 3^n */
    return temp / (seq_t)9;
}
