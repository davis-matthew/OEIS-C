/*
 * A168915  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 44*(a(n-1)+...+a(n-20)) - 989*a(n-21), * a(0)=1, a(1)=46,
 *            a(n)=46*45^(n-1) for 2<=n<=20
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)46;
    if (n <= 20) {
        seq_t res = (seq_t)46;
        for (idx_t i = 2; i <= n; ++i) res *= (seq_t)45;
        return res;
    }

    seq_t buf[21];
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)46;
    for (idx_t i = 2; i <= 20; ++i) buf[i] = buf[i-1] * (seq_t)45;

    for (idx_t i = 21; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= 20; ++j) sum += buf[j];
        seq_t newval = sum * (seq_t)44 - buf[0] * (seq_t)989;
        for (idx_t j = 0; j < 20; ++j) buf[j] = buf[j+1];
        buf[20] = newval;
    }
    return buf[20];
}
