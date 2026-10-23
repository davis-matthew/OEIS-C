/*
 * A033741  Expansion of Product_{d | 29} theta_3(q^d).
 * https://oeis.org/A033741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y) ∈ ℤ² : x² + 29·y² = n}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Integer square‑root floor for 64‑bit unsigned integers
   (no 128‑bit arithmetic, no floating point). */
static inline idx_t sqrt_floor_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = ((idx_t)1) << 32;   /* exclusive upper bound, sqrt(UINT64_MAX) < 2¹⁶⁰ */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid && mid > n / mid)   /* mid*mid would overflow or be > n */
            high = mid;
        else
            low = mid + 1;
    }
    return low - 1;
}

/* Return 2 if t is a positive perfect square, 1 if t == 0, otherwise 0.
   This represents the number of integer solutions x with x² = t. */
static inline seq_t count_sol_x(idx_t t)
{
    if (t == 0)
        return (seq_t)1;                 /* x = 0 only */
    idx_t r = sqrt_floor_u64(t);
    if ((unsigned __int128)r * r == t)
        return (seq_t)2;                 /* ±sqrt(t) */
    return (seq_t)0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t ymax = sqrt_floor_u64(n / 29ULL);   /* |y| ≤ sqrt(n/29) */
    seq_t res = (seq_t)0;

    for (idx_t y = 0; y <= ymax; ++y) {
        idx_t y2 = y * y;                     /* y², fits in 64 bits for n ≤ 10⁴ */
        idx_t t = n - 29ULL * y2;             /* remaining square */
        seq_t cx = count_sol_x(t);

        if (y == 0)                          /* y = 0 counted once */
            res += cx;
        else                                 /* y ≠ 0 gives ±y */
            res += (cx << 1);
    }
    return res;
}
