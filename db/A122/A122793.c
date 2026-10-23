/*
 * A122793  Connell sum sequence (partial sums of the Connell sequence).
 * https://oeis.org/A122793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n² + n - R*((6*n+1)-R²)/6 , where R = round(sqrt(2*n))
 * Range:     u8 n <= 17, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned integer (floor). */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t low  = 0;
    uint64_t high = x;
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        uint64_t mid2 = mid * mid;
        if (mid2 <= x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;          /* avoid underflow */
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    uint64_t N     = (uint64_t)n;
    uint64_t twoN  = N << 1;
    uint64_t R     = isqrt_uint64(twoN);              /* floor(sqrt(2*n)) */
    uint64_t diffLow  = twoN - R * R;
    uint64_t next  = R + 1;
    uint64_t diffHigh = next * next - twoN;
    if (diffLow >= diffHigh) R = next;               /* round to nearest */

    uint64_t term1 = N * N + N;                      /* n² + n */
    uint64_t inner = 6 * N + 1 - R * R;              /* 6*n + 1 - R² */
    uint64_t prod  = R * inner;
    uint64_t div   = prod / 6;                       /* exact division */
    uint64_t res   = term1 - div;

    return (seq_t)res;                               /* cast to target type */
}
