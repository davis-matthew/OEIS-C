/*
 * A194745  Number of k such that {-k*sqrt(5)} > {-n*sqrt(5)}, where { } =
 *          fractional part.
 * https://oeis.org/A194745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{k : 1 ≤ k < n, {k·√5} < {n·√5}}| where {x} is the
 *            fractional part of x. This implementation uses only integer
 *            arithmetic
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute ⌊√x⌋ for a 128‑bit unsigned integer.  */
static unsigned __int128
floor_sqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 64;   /* √x ≤ 2⁶⁴-1  */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;   /* lo == ⌊√x⌋ */
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /*  fn = ⌊n·√5⌋  using  √(5·n²) = n·√5.  */
    unsigned __int128 prod_n = (unsigned __int128)n * n;
    uint64_t fn = (uint64_t)floor_sqrt_u128(5 * prod_n);

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        unsigned __int128 prod_k = (unsigned __int128)k * k;
        uint64_t fk = (uint64_t)floor_sqrt_u128(5 * prod_k);

        unsigned __int128 diff = fn - fk;                     /* fn - fk ≥ 0   */
        unsigned __int128 left = (unsigned __int128)(n - k) * (n - k) * 5;

        /*  {k·√5} < {n·√5}  ⇔  (n‑k)²·5 > (fn‑fk)²  */
        if (left > diff * diff)
            ++count;
    }
    return count;
}
