/*
 * A226646  Number of ways to express m/n as Egyptian fractions in just three
 *          terms, that is, m/n = 1/x + 1/y + 1/z satisfying 1 <= x <= y <= z
 *          and read by antidiagonals.
 * https://oeis.org/A226646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of solutions to m/n = 1/x + 1/y + 1/z, where m/n
 *            is the fraction obtained from the anti‑diagonal index n (m –
 *            row, n – column); solutions are counted with 1 ≤ x ≤ y ≤ z. The
 *            sequence is read by anti‑diagonals of the two‑dimensional table
 *            of counts
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   egypt3_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/egypt3_count.h"

/* Optional helper: compute integer square root (floor) of a 64‑bit
   value.  The build system may provide an equivalent function;
   this local implementation is inherited into the build if needed. */
static idx_t
floor_isqrt_u64(idx_t x)
{
    /* Using a simple Newton iteration. */
    if (x <= 1) return x;
    idx_t s = (idx_t)(x >> 1);
    idx_t t = (s + x / s) >> 1;
    while (t < s) {
        s = t;
        t = (s + x / s) >> 1;
    }
    return s;
}

/* Main sequence function.  Translates the index n into the pair (m,n)
   that lies on the corresponding anti‑diagonal, then counts the
   Egyptian‑fraction representations. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but guard against overflow */

    /* Find s such that s*(s-1)/2 ≥ n.  Use integer arithmetic. */
    idx_t s, prev;
    {
        /* Approximate using floor(sqrt(8*n+1)) and adjust. */
        idx_t rad = floor_isqrt_u64(8 * n + 1);
        s = (rad + 1) / 2;            /* floor((1+sqrt(1+8n))/2) */
        if (s * (s - 1) / 2 < n) {
            ++s;
        }
        prev = (s - 1) * (s - 2) / 2;
    }

    /* Position within the diagonal: 1 … s-1 */
    idx_t pos = n - prev;
    /* According to the required order, the pair is (m,n') = (s-pos, pos) */
    idx_t m = s - pos;
    idx_t n_frac = pos;   /* fraction denominator */

    return egypt3_count(m, n_frac);
}
