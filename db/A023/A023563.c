/*
 * A023563  Convolution of A023531 and A000201.
 * https://oeis.org/A023563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = convolution of A023531 and A000201 * a(n) = Σ_{k=1}^{n}
 *            f(k) * floor((n+1-k)·φ) * where f(k) = 1 if k = m(m+3)/2 for
 *            some m≥1, else 0
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 64‑bit integer.
 * Guaranteed to return floor( sqrt(x) ).  Handles x up to 5·10⁸ safely. */
static inline seq_t floor_sqrt_uint64(uint64_t x)
{
    uint64_t low  = 0;
    uint64_t high = 65536;          /* 65536² = 4'294'967'296 > 5·10⁸   */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}

/* Beatty floor for φ = (1+√5)/2:  floor(n·φ) = floor((n + floor(n·√5))/2)  */
static inline seq_t beatty_floor_phi(idx_t n)
{
    uint64_t prod = 5ULL * (uint64_t)n * (uint64_t)n;   /* n²·5 */
    seq_t   sqrt_val = floor_sqrt_uint64(prod);         /* floor(n·√5) */
    return (seq_t)(( (uint64_t)n + (uint64_t)sqrt_val ) / 2);
}

/* The requested sequence: a(n) = Σ_{m≥1, s≤n}  floor((n+1-s_m)·φ)  */
seq_t a(idx_t n)
{
    uint64_t total = 0;

    for (idx_t m = 1; ; ++m) {
        uint64_t s_val = ((uint64_t)m * (m + 3)) / 2;   /* s = m(m+3)/2 */
        if (s_val > n) break;
        idx_t k = (idx_t)(n + 1 - s_val);
        total += (uint64_t)beatty_floor_phi(k);
    }

    return (seq_t)total;
}
