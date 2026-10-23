/*
 * A167269  Triangle read by rows, Pascal's triangle columns interleaved with
 *          1's.
 * https://oeis.org/A167269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   let r = ⌊(√(8*n+1)-1)/2⌋. Let k = n - r(r+1)/2. Then a(n) = 1 if
 *            k is even; otherwise a(n) = binom(r - (k-1)/2, (k+1)/2)
 * Range:     u8 n <= 72, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static inline idx_t triangular(idx_t r) {
    /* r*(r+1)/2 using 128‑bit to avoid overflow */
    return (idx_t)(((__int128)r * (r + 1ULL)) / 2);
}

seq_t a(idx_t n) {
    /* Find r such that triangular(r) <= n < triangular(r+1) */
    idx_t low  = 0;
    idx_t high = 4294967295ULL;                  /* 2^32-1, safe upper bound */

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2; /* upper mid to avoid endless loop */
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t k = n - triangular(r);

    if ((k & 1U) == 0)          /* even column index -> 1 */
        return (seq_t)1;

    idx_t m = (k - 1U) / 2U;     /* m = (k-1)/2  */
    return binom(r - m, m + 1U);
}
