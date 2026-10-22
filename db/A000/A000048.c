/*
 * A000048  Number of n-bead necklaces with beads of 2 colors and primitive
 *          period n, when turning over is not allowed but the two colors can
 *          be interchanged.
 * https://oeis.org/A000048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   a(n) = (1/(2*n)) * Sum_{odd d divides n} mu(d)*2^(n/d), a(0) = 1
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 71, u128 n <= 136,
 *            big any n
 * Helpers:   pow2_divmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_divmod.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* distinct odd primes of n */
    unsigned long long primes[64];
    int np = 0;
    unsigned long long m = (unsigned long long)n;
    while ((m & 1ULL) == 0) m >>= 1;
    for (unsigned long long p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            primes[np++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) primes[np++] = m;

    unsigned long long two_n = 2ULL * (unsigned long long)n;
    seq_t pos = 0, neg = 0;
    long long rsum = 0;

    /* iterate over squarefree odd divisors d (mu(d) != 0) */
    for (unsigned long long mask = 0; mask < (1ULL << np); mask++) {
        unsigned long long d = 1;
        int bits = 0;
        for (int i = 0; i < np; i++) {
            if (mask & (1ULL << i)) {
                d *= primes[i];
                bits++;
            }
        }
        unsigned long long rem = 0;
        seq_t q = pow2_divmod((unsigned long long)n / d, two_n, &rem);
        if (bits & 1) {
            neg += q;
            rsum -= (long long)rem;
        } else {
            pos += q;
            rsum += (long long)rem;
        }
    }

    seq_t res = pos - neg;
    if (rsum >= 0)
        res += (seq_t)(unsigned long long)(rsum / (long long)two_n);
    else
        res -= (seq_t)(unsigned long long)((-rsum) / (long long)two_n);
    return res;
}
