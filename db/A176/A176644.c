/*
 * A176644  Triangle T(n, k) = 40^(k*(n-k)), read by rows.
 * https://oeis.org/A176644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let n be decomposed as n = m(m+1)/2 + k, 0 ≤ k ≤ m. Then a(n) =
 *            40^(k · (m−k)). This is the triangle T(m,k)=40^(k·(m−k)) read by
 *            rows with offset 0
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 38, u128 n <= 59,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

/* Helper: integer sqrt using binary search (works for all uint64_t). */
static inline uint64_t isqrt_uint64(uint64_t v)
{
    uint64_t low = 0, high = v ? v : 1;      /* high ≥ sqrt(v) */
    uint64_t res = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t sq = mid * mid;            /* mid*mid fits in uint64_t for mid ≤ 65535, but here we use 128‑bit */
        if (mid > 0) {
            unsigned __int128 sq128 = (unsigned __int128)mid * mid;
            if (sq128 <= v) {
                res = mid;
                low = mid + 1;
            } else {
                if (mid == 0) break;
                high = mid - 1;
            }
        } else {
            res = 0;
            break;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row m such that m(m+1)/2 ≤ n < (m+1)(m+2)/2. */
    idx_t low = 0, high = isqrt_uint64(2 * n) + 2;   /* high is safe upper bound */
    idx_t row = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 start = (unsigned __int128)mid * (mid + 1) / 2;
        if (start <= n) {
            row = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Offset within the row. */
    unsigned __int128 start = (unsigned __int128)row * (row + 1) / 2;
    idx_t k = (idx_t)(n - start);

    /* Exponent e = k * (row - k). */
    unsigned __int128 e128 = (unsigned __int128)k * (row - k);
    idx_t e = (idx_t)e128;

    /* Compute 40^e.  (seq_pow handles big integers when required.) */
    return seq_pow((seq_t)40, e);
}
