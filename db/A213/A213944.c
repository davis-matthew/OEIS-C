/*
 * A213944  Triangle read by rows, with column k defined by partial sums of
 *          the finite sequence that contains k three times.
 * https://oeis.org/A213944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, less, easy
 * Formula:   a(n) is obtained by reading a triangle row‑wise. The triangle
 *            entry T(r,k) is defined for row r (>=1) and column k (1 <= k <=
 *            r) as: - 0 if 2*k > r+1 - k if 2*k == r+1 - 2*k if 2*k == r -
 *            3*k otherwise For a given global index n (1‑based) find the
 *            smallest row r with r*(r+1)/2 >= n, then k = n -...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer, returning the floor of sqrt(x). */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)-1;  /* 64‑bit maximum, safe since sqrt(2^128‑1) < 2^64. */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* The sequence is defined for n >= 1. */

    /* Find r: smallest integer with r*(r+1)/2 >= n. */
    unsigned __int128 tmp = (unsigned __int128)n * 8 + 1;
    idx_t sqrt_val = isqrt_u128(tmp);
    idx_t r0 = (sqrt_val - 1) >> 1;      /* floor((sqrt(8n+1)-1)/2). */

    unsigned __int128 cum = (unsigned __int128)r0 * (r0 + 1) / 2;
    idx_t r = (cum < n) ? r0 + 1 : r0;   /* r gives the row containing n. */

    /* Compute column k within row r. */
    unsigned __int128 row_start = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = (idx_t)(n - row_start);

    /* Now compute T(r,k). */
    idx_t two_k = k << 1;

    if (two_k > r + 1)
        return 0;
    if (two_k == r + 1)
        return (seq_t)k;
    if (two_k == r)
        return (seq_t)two_k;
    return (seq_t)(3 * k);
}
