/*
 * A024465  s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [ (n+1)/2 ],
 *          s = (Fibonacci numbers), t = A001950 (upper Wythoff sequence).
 * https://oeis.org/A024465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} F(i) * ( ⌊(n+1‑i)·φ⌋ + (n+1‑i) )
 *            where F(i) is the i‑th Fibonacci number and φ = (1+√5)/2. The
 *            upper Wythoff sequence t(m) = ⌊m·φ⌋ + m is computed via integers
 *            only using the identity ⌊m·φ⌋ = ⌊(m + ⌊√(5m²)⌋)/2⌋
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term; for n==0 we return 0. */
    if (n == 0)
        return 0;

    idx_t k = (n + 1) >> 1;                /* floor((n+1)/2) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t fib_i = fib(i);             /* Fibonacci number s(i) */

        idx_t m = n + 1 - i;              /* term index for t */
        seq_t mm = (seq_t)m;              /* cast to seq_t for computations */

        /* Compute floor(m * φ)  using integer arithmetic:
           floor(m * φ) = floor( (m + floor(√(5 m²)) ) / 2 ) */
        seq_t sqrt_term = floor_sqrt((seq_t)5 * mm * mm);
        seq_t floor_mphi = (mm + sqrt_term) / 2;

        seq_t t_m = floor_mphi + mm;      /* upper Wythoff number t(m) */

        sum += fib_i * t_m;
    }

    return sum;
}
