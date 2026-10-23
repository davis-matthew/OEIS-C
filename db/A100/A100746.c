/*
 * A100746  Inverse modulo 2 binomial transform of Jacobsthal numbers J(n).
 * https://oeis.org/A100746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^A010060(n-k) * (binomial(n, k) mod 2) *
 *            A001045(k)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A010060(x) is the binary weight of x, and A001045(k) are Jacobsthal numbers. */

#include <stdint.h>          /* for uint64_t etc. */

seq_t a(idx_t n) {
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    seq_t pow2k   = 1;            /* 2^k, starting with k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(n,k) is odd iff (k & (n-k)) == 0  (Lucas theorem) */
        if ((k & (n - k)) == 0) {
            /* Jacobsthal number J(k) = (2^k - (-1)^k)/3  */
            seq_t Jk;
            if (k & 1) {           /* k is odd */
                Jk = (pow2k + 1) / 3;
            } else {
                Jk = (pow2k - 1) / 3;
            }

            /* sign = (-1)^{popcount(n-k)} */
            idx_t m = n - k;
            unsigned long long pop = __builtin_popcountll((unsigned long long)m);
            if ((pop & 1ULL) == 0) {
                sum_pos += Jk;    /* positive contribution */
            } else {
                sum_neg += Jk;    /* negative contribution */
            }
        }

        /* advance to next power of two: 2^{k+1} = 2 * 2^k */
        pow2k <<= 1;
    }

    /* The result is guaranteed non‑negative, so the subtraction will not underflow. */
    return (sum_pos >= sum_neg) ? (sum_pos - sum_neg) : 0;
}
