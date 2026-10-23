/*
 * A122177  Triangle, read by rows, where T(n,k) = C( k*(k+1)/2 + n-k + 2,
 *          n-k) for n>=k>=0.
 * https://oeis.org/A122177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r,k) * where r = floor((sqrt(8n+1)-1)/2) * k = n -
 *            r*(r+1)/2 * T(r,k) = C( k*(k+1)/2 + r - k + 2 , r - k ), *
 *            C(n,k) is the binomial coefficient
 * Range:     u8 n <= 30, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* multiplicative binomial coefficient: C(n,k).
 *  Assumes 0 <= k <= n and both arguments fit in idx_t. */
static inline seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* result * (n - k + i) / i   (exact division) */
        result = result * ((seq_t)n - k + i) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* --- Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = (seq_t)mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* --- Column index within the row. */
    idx_t k = n - (idx_t)((seq_t)r * (r + 1) / 2);

    /* --- Compute T(r,k). */
    seq_t top    = (seq_t)k * (k + 1) / 2 + (seq_t)r - (seq_t)k + 2;
    idx_t bottom = r - k;   /* same as n-k in the definition */

    return binomial((idx_t)top, bottom);
}
