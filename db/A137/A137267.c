/*
 * A137267  Chung-Graham juggling polynomials as a triangular sequence of
 *          positive coefficients.
 * https://oeis.org/A137267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Determine the row r such that T(r‑1) < n ≤ T(r) (T(k)=k(k+1)/2).
 *            Let pos = n – T(r‑1) – 1 (0‑based position inside the row). For
 *            r = 1 the only entry is 1. For r ≥ 2: • If pos is 0 or 1, the
 *            value is simply (r‑1). • Otherwise the value is (r – pos) * pos!
 *            where pos! is the factorial of pos
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 104, u64 n <= 252,
 *            u128 n <= 664, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle empty input silently (though n ≥ 1 by definition). */
    if (n == 0) return 0;

    /* Find the row r containing the nth element. */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < n)
        ++r;

    /* If it's the first row, return its single element. */
    if (r == 1)
        return (seq_t)1;

    /* Determine position within the row (0‑based). */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t pos = n - prev_tri - 1;   /* 0 ≤ pos < r */

    /* First two entries of every row (except the very first row)
       are equal to (r‑1). */
    if (pos < 2)
        return (seq_t)(r - 1);

    /* pos ≥ 2 → compute (r – pos) * pos! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= pos; ++i)
        fact *= (seq_t)i;

    return (seq_t)(r - pos) * fact;
}
