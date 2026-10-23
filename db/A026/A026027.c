/*
 * A026027  a(n) = number of (s(0), s(1), ..., s(2*n-1)) such that s(i) is a
 *          nonnegative integer and |s(i) - s(i-1)| = 1 for i = 1,2,...,n,
 *          s(0) = 3, s(2*n-1) = 6. Also a(n) = T(2*n-1,n-2), where T is
 *          defined in A026022.
 * https://oeis.org/A026027
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2*n-1, n-2) - C(2*n-1, n-6) (n ≥ 2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* --- Small binomial helper -------------------------------------------------
 * Computes C(n,k) without intermediate overflow.
 * For 64-bit seq_t it uses a 128‑bit accumulator.
 * For larger seq_t it relies on the built‑in arithmetic of seq_t. */
static seq_t binom_custom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */

    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        /* 64‑bit build: use __int128 as temporary */
        unsigned __int128 res = 1;
        for (idx_t i = 1; i <= k; ++i) {
            unsigned __int128 num = res * (uint64_t)(n - k + i);
            res = num / i;
        }
        return (seq_t)res;
    } else {
        /* arbitrary‑precision build: use seq_t operations */
        seq_t res = 1;
        for (idx_t i = 1; i <= k; ++i) {
            /* res * (n - k + i) is exact; divide by i next */
            res = (res * (n - k + i)) / i;
        }
        return res;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    idx_t total = 2 * n - 1;
    seq_t term1 = binom_custom(total, n - 2);

    if (n >= 6) {
        seq_t term2 = binom_custom(total, n - 6);
        return term1 - term2;
    } else {
        return term1;
    }
}
