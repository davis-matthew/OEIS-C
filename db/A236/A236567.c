/*
 * A236567  a(n) = |{0 < k < n - 2: k + phi(n-k)/2 is a square}|, where phi(.)
 *          is Euler's totient function.
 * https://oeis.org/A236567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n-2 : k + phi(n-k)/2 is a square}|, computed by
 *            brute force search
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t phi_seq(seq_t n)
{
    if (n <= 1) return 1;          /* phi(0)=1, phi(1)=1 for our use */
    seq_t result = n;
    seq_t temp = n;

    /* handle factor 2 */
    if ((temp & 1) == 0) {
        result -= result / 2;
        while ((temp & 1) == 0) temp >>= 1;
    }

    /* odd factors */
    seq_t p = 3;
    seq_t sqrt_temp = floor_sqrt(temp);
    while (p <= sqrt_temp && temp > 1) {
        if (temp % p == 0) {
            result -= result / p;
            while (temp % p == 0) temp /= p;
            sqrt_temp = floor_sqrt(temp);
        }
        p += 2;
    }
    if (temp > 1) {
        result -= result / temp;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* no k satisfies 0<k<n-2 */

    seq_t count = 0;

    for (idx_t k = 1; k <= n - 3; ++k) {
        seq_t m = (seq_t)(n - k);             /* n - k >= 3 */
        seq_t phi_m = phi_seq(m);
        seq_t sum = (seq_t)k + phi_m / 2;

        seq_t sq = floor_sqrt(sum);
        if (sq * sq == sum)
            ++count;
    }
    return count;
}
