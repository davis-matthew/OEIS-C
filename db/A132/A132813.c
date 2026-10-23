/*
 * A132813  Triangle read by rows: T(n,k) = binomial(n, k)*binomial(n+1, k).
 * https://oeis.org/A132813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, k) * binomial(r+1, k) where r = ⌊(-1 +
 *            √(1+8n)) / 2⌋ and k = n - r(r+1)/2. This follows from the
 *            triangle T(ℓ, k) = C(ℓ, k)*C(ℓ+1, k) read by rows (row ℓ has ℓ+1
 *            entries)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 179, u64 n <= 645,
 *            u128 n <= 2310, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"          /* binomial(idx_t n, idx_t k) */

static idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n) {
    /* compute r = floor((-1 + sqrt(1+8n)) / 2)  */
    idx_t iv = 1 + (n << 3);              /* 1 + 8*n  (assumes no overflow for n < 2^61) */
    idx_t root = isqrt_u64(iv);           /* floor sqrt(1+8n) */
    idx_t r = (root - 1) >> 1;            /* integer division by 2 */

    /* k = n - r(r+1)/2  */
    idx_t start_of_row = r * (r + 1) / 2;
    idx_t k = n - start_of_row;

    /* compute T(r, k) = C(r, k) * C(r+1, k)  */
    seq_t bin1 = binomial(r, k);
    seq_t bin2 = binomial(r + 1, k);
    return bin1 * bin2;
}
