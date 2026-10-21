/*
 * A080108  a(n) = Sum_{k=1..n} k^(n-k)*binomial(n-1,k-1).
 * https://oeis.org/A080108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^(n-k) * binomial(n-1,k-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t res = 1;
    seq_t cur = base;
    while (exp) {
        if (exp & 1) res *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    seq_t bin = 1;   /* binomial(n-1,0) */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t powk = pow_seq_local((seq_t)k, n - k);
        sum += powk * bin;
        if (k < n) {
            bin = (bin * (n - k)) / k;
        }
    }
    return sum;
}
