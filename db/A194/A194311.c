/*
 * A194311  Triangular array: g(n,k)=number of fractional parts (i*e) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n^2, 1<=k<=n.
 * https://oeis.org/A194311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = g(m,k) where m and k are derived from the position n in
 *            the * triangular array. g(m,k) counts i (1≤i≤m²) with the
 *            fractional part of * i·e in the interval [(k−1)/m, k/m). The
 *            implementation uses a fixed‑point * approximation of e (e ≈
 *            2.718281828459045…) to avoid floating point
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fixed‑point constants:  Q = 10¹³  and  ADD = floor(e·Q) mod Q. */
static const uint64_t Q   = 10000000000000ULL;   /* 10^13  */
static const uint64_t ADD = 7182818284590ULL;    /* floor(e*Q) % Q */

seq_t a(idx_t n)
{
    /* -----------------------------------------------------------------
     *  Find the row number m and the column index k from the linear
     *  index n (rows are flattened, row m contains m terms).
     * ----------------------------------------------------------------- */
    idx_t m = 1;
    while ((seq_t)m * (m + 1) / 2 < n) ++m;      /* binary search could be used */

    idx_t prev = (idx_t)((m - 1) * m / 2);        /* total terms before row m */
    idx_t k    = n - prev;                       /* 1‑based column within row m */

    /* -----------------------------------------------------------------
     *  Count i with fractional part of i·e in the required interval.
     *  The fractional part is approximated by the value
     *      r_i ≈ (i*D) mod Q   / Q       where D = floor(e*Q).
     *  Since Q=10¹³, the error in r_i is < 1e‑5 for i ≤ 10⁸, which is
     *  far below the smallest interval width 1/m (≥10⁻⁴ for m ≤ 10000).
     * ----------------------------------------------------------------- */
    idx_t limit = m * m;                        /* i runs from 1 to m² */
    uint64_t rem = 0;                           /* holds (i*D) mod Q           */

    /* Bounds in the fixed‑point representation of the interval.   */
    uint64_t low  = (uint64_t)(k - 1) * Q;
    uint64_t high = (uint64_t)k * Q;

    seq_t cnt = 0;
    for (idx_t i = 0; i < limit; ++i) {
        rem += ADD;                     /* advance to next i*D   */
        if (rem >= Q) rem -= Q;         /* keep remainder < Q    */

        uint64_t prod = rem * m;        /* (i*D mod Q) * m ≈ m·{i·e}·Q */
        if (prod >= low && prod < high) ++cnt;   /* lies in the k‑th bin   */
    }

    return cnt;
}
