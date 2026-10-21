/*
 * A053591  Closed 3-dimensional ball numbers (version 1): a(n)= number of
 *          integer points (i,j,k) contained in a closed ball of diameter n,
 *          centered at (0,0,0).
 * https://oeis.org/A053591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Distance‑ball lattice point count.
   a(n) = |{(i,j,k)∈ℤ³ : i² + j² + k² ≤ ⌊n²/4⌋}|   (closed ball of diameter n) */

#include <stdint.h>

/* Integer square root for arbitrary seq_t using binary search */
static inline seq_t seq_isqrt(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x;
    while (lo < hi)
    {
        seq_t mid = (lo + hi + 1) / 2;          /* upper mid to avoid infinite loop */
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;                                 /* lo == floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* R = ⌊n²/4⌋  –  radius squared (in integer units)                */
    seq_t R = ((seq_t)n * (seq_t)n) / 4;

    seq_t total = 0;

    seq_t i_max = seq_isqrt(R);
    idx_t lim_i = (idx_t)i_max;                 /* safe: i_max ≤ n/2 for all n */

    for (idx_t i = 0; i <= lim_i; ++i)
    {
        seq_t i_s = (seq_t)i;
        seq_t i2   = i_s * i_s;
        seq_t rem_i = R - i2;

        seq_t j_max = seq_isqrt(rem_i);
        idx_t lim_j = (idx_t)j_max;

        for (idx_t j = 0; j <= lim_j; ++j)
        {
            seq_t j_s = (seq_t)j;
            seq_t j2  = j_s * j_s;
            seq_t rem  = rem_i - j2;

            seq_t kmax = seq_isqrt(rem);
            seq_t count_k = 2 * kmax + 1;        /* k = -kmax … +kmax */

            seq_t mult;
            if (i == 0 && j == 0)
                mult = 1;
            else if (i == 0 || j == 0)
                mult = 2;
            else
                mult = 4;

            total += count_k * mult;
        }
    }

    return total;
}
