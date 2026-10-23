/*
 * A176775  Index of n as m-gonal number for the smallest possible m
 *          (=A176774(n)).
 * https://oeis.org/A176775
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A176775:
   Index of n as an m‑gonal number for the smallest possible m (A176774(n)).
   Formula: for n ≥ 3 let m be the smallest integer ≥ 3 such that
            D = (m‑4)² + 8 · (m‑2) · n is a perfect square.
            Then a(n) = (m‑4 + √D) / (2·m‑4).  */

/* Integer square root for unsigned 64‑bit values. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid != 0 && mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;          /* sequence is defined for n ≥ 3 */

    for (idx_t m = 3; m <= n; ++m) {
        idx_t a_coeff = m - 2;          /* (m‑2) */
        idx_t b_coeff = m - 4;          /* (m‑4) */
        idx_t D = b_coeff * b_coeff + 8 * a_coeff * n;   /* discriminant */

        idx_t sqrtD = isqrt_u64(D);
        if (sqrtD * sqrtD != D)          /* not a perfect square */
            continue;

        idx_t num   = b_coeff + sqrtD;    /* numerator in the formula */
        idx_t denom = 2 * a_coeff;        /* denominator in the formula */
        if (num % denom != 0)
            continue;

        idx_t k = num / denom;          /* index of n in the m‑gonal sequence */
        if (k >= 1)
            return (seq_t)k;
    }

    /* m = n always works with k = 2; the loop always terminates before
       reaching this point, but return 2 as a safe default. */
    return 2;
}
