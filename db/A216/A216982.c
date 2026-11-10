/*
 * A216982  Anti-Chowla's function: sum of anti-divisors of n except the
 *          largest.
 * https://oeis.org/A216982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A066417(n) - A066481(n), where A066417(n) = sigma(2n-1) +
 *            sigma(2n+1) + 2^(k+1)*sigma(n/2^k) - 6n - 2 (k = v2(n)),
 *            A066481(n) = floor((2n+1)/3); a(1) = a(2) = 0
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9965 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t A216982_sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    idx_t kk = 0;
    idx_t temp = n;
    while ((temp & 1ULL) == 0) {
        ++kk;
        temp >>= 1;
    }

    idx_t two_n = n << 1;
    seq_t s1 = A216982_sigma_of(two_n - 1);
    seq_t s3 = A216982_sigma_of(two_n + 1);
    seq_t s2 = A216982_sigma_of(n >> kk);
    seq_t p2 = ((seq_t)1) << (kk + 1);

    seq_t total = s1 + s3 + s2 * p2 - ((seq_t)6 * (seq_t)n) - (seq_t)2;
    seq_t largest = ((seq_t)n * 2 + 1) / 3;

    return total - largest;
}
