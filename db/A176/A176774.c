/*
 * A176774  Smallest polygonality of n = smallest integer m>=3 such that n is
 *          m-gonal number.
 * https://oeis.org/A176774
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest m ≥ 3 such that n is an m‑gonal number; i.e.
 *            there exists k ≥ 1 with ((m‑2) k² – (m‑4) k)/2 = n
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Behaviour for n < 3 is irrelevant for the OEIS offset. */
    if (n < 3) return 0;

    for (idx_t m = 3; m <= n; ++m) {
        seq_t m_seq        = (seq_t)m;
        seq_t m_minus_2    = m_seq - 2;      /* a = m–2  */
        seq_t m_minus_4    = m_seq - 4;      /* m–4     */

        /* Discriminant D = (m‑4)² + 8 n (m‑2) */
        seq_t term1 = m_minus_4 * m_minus_4;
        seq_t term2 = (seq_t)8 * (seq_t)n * m_minus_2;
        seq_t D = term1 + term2;

        seq_t sqrtD = floor_sqrt(D);
        if (sqrtD * sqrtD != D) continue;          /* D must be a perfect square */

        seq_t numerator   = m_minus_4 + sqrtD;
        seq_t denominator = (seq_t)2 * m_minus_2;   /* 2 · (m‑2) */

        if (numerator % denominator != 0) continue; /* k not integer */

        seq_t k = numerator / denominator;
        if (k >= 1) return (seq_t)m;                /* minimal polygonality found */
    }

    /* Fallback: n is always an n‑gonal number (k = 2). */
    return (seq_t)n;
}
