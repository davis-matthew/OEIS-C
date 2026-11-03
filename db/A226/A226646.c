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
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Count the number of Egyptian fraction representations of m/n
   in exactly three terms with 1 ≤ x ≤ y ≤ z. */
static seq_t
egypt3_count(idx_t m, idx_t n)
{
    seq_t cnt = 0;
    /* Lower bound for x: 1/x < m/n  ->  x > n/m */
    idx_t minx = n / m + 1;            /* integer division gives floor */
    /* Upper bound for x: 3/x ≤ m/n  ->  x ≤ 3n/m  */
    idx_t maxx = (3 * n) / m;          /* floor division */

    for (idx_t x = minx; x <= maxx; ++x) {
        /* For fixed x the denominator of z must be positive: m*x - n > 0 */
        idx_t mx_minus_n = m * x - n;
        if (mx_minus_n == 0) continue;   /* denominator would be zero */

        /* Lower bound for y: y > n*x/(m*x - n) and y ≥ x */
        idx_t num1 = n * x;
        idx_t den1 = mx_minus_n;
        idx_t y_low = num1 / den1 + 1;   /* floor + 1 => strict > */
        if (y_low < x) y_low = x;

        /* Upper bound for y from z ≥ y:
           y ≤ 2n*x/(m*x - n) */
        idx_t y_high = (2 * n * x) / den1;
        if (y_high < y_low) continue;

        for (idx_t y = y_low; y <= y_high; ++y) {
            /* Compute denominator of z */
            uint64_t denom =
                (uint64_t)m * x * y - (uint64_t)n * y - (uint64_t)n * x;
            if (!denom) continue;       /* avoid division by zero */
            uint64_t numer = (uint64_t)n * x * y;
            if (numer % denom) continue; /* z must be integer */
            uint64_t z = numer / denom;
            if (z < y) continue;        /* maintain x ≤ y ≤ z */
            ++cnt;
        }
    }
    return cnt;
}

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
