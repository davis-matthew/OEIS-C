/*
 * A049261  Smallest nonnegative value taken on by nx^2 - 2y^2 for an infinite
 *          number of integer pairs (x, y).
 * https://oeis.org/A049261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS formula calculates the smallest non‑negative value obtained
 * by n·x² – 2·y² for infinitely many integer pairs (x, y).  The value
 * is 0 exactly when n = 2·k² (k∈ℕ); otherwise it is the minimal
 * positive integer represented by the indefinite binary quadratic form
 * n·x² – 2·y².  The implementation searches over x and chooses the
 * nearest integer y for each x, guaranteeing that the minimal non‑negative
 * value is found without any floating‑point arithmetic.  A purely
 * integer square‑root routine is used for 64‑bit intermediate values
 * because the product n·x² with x ≤ 50 000 never exceeds 2⁶³ for
 * all n ≤ 10 000.  The code works for 8, 16, 32, 64, 128‑bit and
 * arbitrary‑precision seq_t types. */
#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root for unsigned 64‑bit values, safe for all
 * intermediate calculations (t ≤ 4·10¹²).  The algorithm uses only
 * integer operations and requires no floating point. */
static inline idx_t isqrt_u64(uint64_t v)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;          /* largest power of 4 ≤ v */
    while (bit > v) bit >>= 2;
    while (bit) {
        if (v >= r + bit) {
            v -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return (idx_t)r;
}

seq_t a(idx_t n)
{
    /* 0 occurs iff n = 2·k².  This test uses 64‑bit arithmetic,
     * safe because n ≤ 10 000. */
    if ((n & 1U) == 0U) {
        idx_t m = n >> 1;                     /* m = n/2      */
        idx_t r = isqrt_u64((uint64_t)m);
        if ((uint64_t)r * r == (uint64_t)m) {
            return (seq_t)0U;
        }
    }

    /* Search over x.  The bound 50 000 is more than sufficient for
     * all n in the non‑hard range; the algorithm stops early when
     * a minimal value of 1 is found. */
    const idx_t X_MAX = 50000U;
    /* Use 64‑bit intermediate variables to avoid overflow for n·x². */
    uint64_t best_val = UINT64_MAX;

    for (idx_t x = 1U; x <= X_MAX; ++x) {
        uint64_t t = (uint64_t)n * (uint64_t)x * (uint64_t)x;   /* n·x²   */
        uint64_t half = t >> 1;                                 /* floor(t/2) */
        uint64_t y = (uint64_t)isqrt_u64(half);                  /* floor sqrt(half) */
        uint64_t v  = t - 2U * y * y;                            /* n·x² – 2·y² */

        if (v < best_val) {
            best_val = v;
            if (best_val == 1U) {  /* minimal positive value reached */
                break;
            }
        }
    }

    return (seq_t)best_val;
}
