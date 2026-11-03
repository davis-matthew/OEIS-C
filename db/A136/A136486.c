/*
 * A136486  Number of unit square lattice cells enclosed by origin centered
 *          circle of diameter 2n+1.
 * https://oeis.org/A136486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4 * Σ_{k=1..n} ⌊√((n+½)² – k²)⌋, which simplifies to a(n)
 *            = 4 * Σ_{k=1..n} ⌊√(n² + n – k²)⌋
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 732, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 972 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t n_seq = n;
    seq_t nn = n_seq * (n_seq + (seq_t)1); /* n² + n */

    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = k;
        seq_t val = nn - k_seq * k_seq;     /* n² + n – k² >= 0 */
        seq_t s = isqrt_seq(val);
        sum += s;
    }
    return sum * (seq_t)4;
}
