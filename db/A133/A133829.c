/*
 * A133829  a(n) = the largest "non-isolated divisor" of 2n. A positive
 *          divisor k of n is non-isolated if k-1 or k+1 also divides n.
 * https://oeis.org/A133829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest divisor k of 2n such that k-1 | 2n or k+1 |
 *            2n
 * Range:     u8 n <= 5245, u16 n <= 9479, u32 n <= 9479, u64 n <= 9479,
 *            u128 n <= 9479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* integer square root for 64‑bit unsigned values */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1, high = n, ans = 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0 – safe value */

    idx_t N2 = (idx_t)2 * n;               /* 2n as 64‑bit unsigned */
    seq_t N  = (seq_t)n * 2;               /* 2n as seq_t, per requirement */

    seq_t best = (seq_t)2;                 /* divisor 2 is always non‑isolated */

    idx_t sqrtN = isqrt_u64(N2);

    for (idx_t i = 1; i <= sqrtN; ++i) {
        if (N2 % i != 0) continue;

        idx_t d1 = i;
        idx_t d2 = N2 / i;

        seq_t kd1 = (seq_t)d1;
        bool cond1 = ((kd1 > (seq_t)1) && ((N % (kd1 - (seq_t)1)) == (seq_t)0))
                     || ((N % (kd1 + (seq_t)1)) == (seq_t)0);
        if (cond1 && kd1 > best) best = kd1;

        if (d2 != d1) {
            seq_t kd2 = (seq_t)d2;
            bool cond2 = ((kd2 > (seq_t)1) && ((N % (kd2 - (seq_t)1)) == (seq_t)0))
                         || ((N % (kd2 + (seq_t)1)) == (seq_t)0);
            if (cond2 && kd2 > best) best = kd2;
        }
    }

    return best;
}
