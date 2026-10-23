/*
 * A117919  Triangle read by rows: T(n, k) = 2^floor((k-1)/2)*binomial(n-1,
 *          k-1).
 * https://oeis.org/A117919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^floor((k-1)/2) * binomial(r-1, k-1) where r is the
 *            unique integer with r(r-1)/2 < n ≤ r(r+1)/2 and k = n - r(r-1)/2
 *            (row and column indices in the triangle)
 * Range:     u8 n <= 40, u16 n <= 128, u32 n <= 420, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* safety, though offset starts at 1 */

    /* Find row r such that r(r-1)/2 < n ≤ r(r+1)/2. */
    idx_t low = 1, high = n;
    unsigned __int128 target = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum_mid = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum_mid >= target) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;

    /* Column k within that row. */
    unsigned __int128 prev_sum = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = n - prev_sum;          /* 1‑based column index */

    /* Compute 2^floor((k-1)/2). */
    idx_t exp = (k - 1) / 2;
    seq_t pow2exp = pow2(exp);

    /* Compute binomial coefficient C(r-1, k-1). */
    seq_t bin = binom(r - 1, k - 1);

    return pow2exp * bin;
}
