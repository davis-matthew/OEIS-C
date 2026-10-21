/*
 * A224226  a(0)=1; thereafter a(n) =s(n,3)-s(n,4)-s(n,6)+s(n,12), where
 *          s(n,k) = sigma(n/k) if k divides n, otherwise 0.
 * https://oeis.org/A224226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = s(n,3) - s(n,4) - s(n,6) + s(n,12), where s(n,k) =
 *            sigma(n/k) if k | n else 0, and a(0)=1
 * Range:     i8 n <= 269, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma_of_idx(idx_t m)
{
    if (m == 0)
        return 0;
    seq_t sum = 1;
    idx_t temp = m;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            seq_t term = 1;
            seq_t term_pow = 1;
            while (temp % p == 0) {
                temp /= p;
                term_pow *= p;
                term += term_pow;
            }
            sum *= term;
        }
    }
    if (temp > 1) {
        sum *= (1 + temp);
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t res = 0;
    if (n % 3 == 0)
        res += sigma_of_idx(n / 3);
    if (n % 4 == 0)
        res -= sigma_of_idx(n / 4);
    if (n % 6 == 0)
        res -= sigma_of_idx(n / 6);
    if (n % 12 == 0)
        res += sigma_of_idx(n / 12);
    return res;
}
