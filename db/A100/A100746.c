/*
 * A100746  Inverse modulo 2 binomial transform of Jacobsthal numbers J(n).
 * https://oeis.org/A100746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^A010060(n-k) * (binomial(n, k) mod 2) *
 *            A001045(k)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 130,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 979 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A010060(x) is the binary weight of x, and A001045(k) are Jacobsthal numbers. */

#include <stdint.h>          /* for uint64_t etc. */

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

seq_t a_wide(idx_t n) {
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

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
