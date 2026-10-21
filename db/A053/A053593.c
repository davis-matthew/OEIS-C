/*
 * A053593  Closed 3-dimensional ball numbers (version 2): a(n)= number of
 *          integer points (i,j,k) contained in a closed ball of diameter n,
 *          centered at (1/2,0,0).
 * https://oeis.org/A053593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of integer triples (i,j,k) such that (i – 1/2)^2 +
 *            j^2 + k^2 <= (n/2)^2
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: returns floor(sqrt(x)) */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;                 /* highest power of 4 <= x */
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* use 64‑bit arithmetic for intermediate calculations */
    uint64_t n64   = (uint64_t)n;
    uint64_t n2    = n64 * n64;              /* n^2  */
    seq_t total = 0;

    /* iterate over all admissible i.
       (2i-1)^2 <= n^2 guarantees (i-1/2)^2 <= (n/2)^2. */
    int64_t limit = (int64_t)((n + 1) / 2);      /* positive limit */
    for (int64_t i = -limit; i <= limit; ++i) {
        int64_t two_i_minus1 = 2 * i - 1;
        int64_t left = (int64_t)n2 - (int64_t)two_i_minus1 * two_i_minus1;
        if (left < 0) continue;                 /* outside the sphere */

        uint64_t t_int = (uint64_t)left / 4;      /* floor( (n^2-(2i-1)^2)/4 ) */
        uint64_t f = isqrt_u64(t_int);           /* max |j| for this i */

        /* partial count for this i */
        seq_t partial = (seq_t)(1 + 4 * f);      /* (2*f+1) + 2*f  */
        seq_t sumK = 0;

        uint64_t kmax = f;
        for (uint64_t j = 1; j <= f; ++j) {
            uint64_t jj = j * j;
            if (jj > t_int) break;
            uint64_t rem = t_int - jj;
            while (kmax > 0 && kmax * kmax > rem) --kmax;
            sumK += (seq_t)kmax;
        }
        partial += (seq_t)(4 * sumK);
        total += partial;
    }
    return total;
}
