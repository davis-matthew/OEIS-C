/*
 * A100852  Triangle read by rows: T(n,k) = 2^k * 3^n, 0 <= k <= n.
 * https://oeis.org/A100852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3^r * 2^k where r = floor((sqrt(8*n+1)-1)/2) is the row
 *            index and k = n - r*(r+1)/2 is the column index within that row
 * Range:     u8 n <= 11, u16 n <= 32, u32 n <= 102, u64 n <= 349,
 *            u128 n <= 1323, big any n
 * Validated: standard, pass, 60 terms through n = 9828 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row number r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static inline idx_t row_of_idx(idx_t n) {
    idx_t lo = 0, hi = 1;
    /* Increase hi until the triangular number exceeds n. */
    while ((unsigned __int128)hi * (hi + 1) / 2 <= n) {
        if (hi > (idx_t)(-1) / 2) {     /* protect against overflow of hi */
            hi = (idx_t)(-1);
            break;
        }
        hi <<= 1;
    }
    /* Binary search between lo and hi. */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Compute 3^exp and 2^exp by exponentiation by squaring. */
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1ul) result *= base;
        base *= base;
        exp >>= 1ul;
    }
    return result;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    idx_t r = row_of_idx(n);
    idx_t tri = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - tri;
    seq_t pow3 = pow_seq((seq_t)3, r);
    seq_t pow2 = pow_seq((seq_t)2, k);
    return pow3 * pow2;
}
