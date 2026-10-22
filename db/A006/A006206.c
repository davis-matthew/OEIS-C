/*
 * A006206  Number of aperiodic binary necklaces of length n with no
 *          subsequence 00, excluding the necklace "0".
 * https://oeis.org/A006206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/n) * Sum_{d|n} mu(n/d) * Lucas(d), with Lucas(0) = 2,
 *            Lucas(1) = 1, Lucas(k) = Lucas(k-1) + Lucas(k-2). Each Lucas(k)
 *            is kept as a pair (q, r) with Lucas(k) = q*n + r and 0 <= r < n,
 *            so no intermediate is much larger than a(n)
 * Range:     u8 n <= 17, u16 n <= 30, u32 n <= 54, u64 n <= 101,
 *            u128 n <= 195, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int moebius(uint64_t n) {
    int mu = 1;
    uint64_t d = 2;
    if (n == 1) return 1;
    while (d * d <= n) {
        if (n % d == 0) {
            n /= d;
            if (n % d == 0) return 0;
            mu = -mu;
        } else {
            d++;
        }
    }
    if (n > 1) mu = -mu;
    return mu;
}

seq_t a(idx_t n) {
    seq_t pos_q = 0, neg_q = 0;
    long long rsum = 0;
    seq_t q0, q1;
    uint64_t r0, r1;
    idx_t k;

    /* k = 0: Lucas(0) = 2 */
    q0 = (seq_t)(2 / n);
    r0 = 2 % n;
    /* k = 1: Lucas(1) = 1 */
    q1 = (seq_t)(1 / n);
    r1 = 1 % n;

    for (k = 1; k <= n; k++) {
        if (k >= 2) {
            seq_t q2 = q0 + q1;
            uint64_t r2 = r0 + r1;
            if (r2 >= n) {
                r2 -= n;
                q2 += 1;
            }
            q0 = q1;
            r0 = r1;
            q1 = q2;
            r1 = r2;
        }
        /* q1, r1 now represent Lucas(k) */
        if (n % k == 0) {
            int mu = moebius(n / k);
            if (mu > 0) {
                pos_q += q1;
                rsum += (long long)r1;
            } else if (mu < 0) {
                neg_q += q1;
                rsum -= (long long)r1;
            }
        }
    }

    {
        seq_t res = pos_q - neg_q;
        long long t = rsum / (long long)n;
        if (t >= 0)
            res += (seq_t)(unsigned long long)t;
        else
            res -= (seq_t)(unsigned long long)(-t);
        return res;
    }
}
