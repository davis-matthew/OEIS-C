/*
 * A054172  Inverse Moebius transform of A000048 (starting at term 0).
 * https://oeis.org/A054172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d | n+1} A000048(d-1), where A000048(k) = (1/(2k)) *
 *            Sum_{odd d | k} mu(d)*2^(k/d) for k >= 1 and A000048(0) = 1
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor(2^k / m) as seq_t, with remainder in *rem (computed by long division) */
static seq_t A000048_pow2_divmod(unsigned long long k, unsigned long long m, unsigned long long *rem)
{
    seq_t q = (seq_t)(1ULL / m);
    unsigned long long r = 1ULL % m;
    unsigned long long i;
    for (i = 0; i < k; i++) {
        q = q + q;
        r = r + r;
        if (r >= m) {
            r -= m;
            q += 1;
        }
    }
    *rem = r;
    return q;
}

static seq_t A000048_a(idx_t n)
{
    if (n == 0) return (seq_t)1;

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
        seq_t q = A000048_pow2_divmod((unsigned long long)n / d, two_n, &rem);
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

seq_t a(idx_t n)
{
    unsigned long long m = (unsigned long long)n + 1ULL;
    seq_t total = 0;
    for (unsigned long long d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            unsigned long long e = m / d;
            total += A000048_a((idx_t)(d - 1));
            if (e != d) total += A000048_a((idx_t)(e - 1));
        }
    }
    return total;
}
