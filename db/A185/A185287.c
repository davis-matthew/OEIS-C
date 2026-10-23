/*
 * A185287  R(m,n) is the number of ways to split two strings x and y of
 *          length m and n, respectively, into the same number of nonempty
 *          parts such that at least one of the corresponding parts has length
 *          1 and such that the parts of the y string have at most size 2.
 * https://oeis.org/A185287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   binom, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A185287: a(n) = R(m,n) where (m,n) are obtained by
 * antidiagonal enumeration of the table of R(m,n):
 *   first term a(1)=R(1,1),
 *   subsequent terms list the entries of the table along
 *   diagonals s = m+n with decreasing m (increasing n).
 * Formula used: R(m,n) = C(m-1,n-1) +
 *      Σ_{k=2}^{n-1} C(m+k-n-1, 2k-n-1)*C(k, 2k-n).
 */
#include <stdint.h>
#include "../utils/binom.h"      /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/floor_sqrt.h" /* seq_t floor_sqrt(seq_t x); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, but safe fallback */

    /* Find diagonal s such that (s-1)(s-2)/2 < n ≤ s(s-1)/2,  s ≥ 2 */
    idx_t disc = 1 + 8 * n;                    /* 64-bit safe for n≤10000 */
    seq_t sq = floor_sqrt((seq_t)disc);
    idx_t s = (1 + (idx_t)sq) / 2;             /* integer division, floor */

    /* If not enough, step to next diagonal */
    if (s * (s - 1) / 2 < n) ++s;

    /* Position inside this diagonal (0‑based) */
    idx_t prev = (s - 1) * (s - 2) / 2;
    idx_t offset = n - prev - 1;               /* 0 ≤ offset < s-1 */

    /* Coordinates of the entry: m decreases, n increases */
    idx_t m   = s - 1 - offset;                /* m ≥ 1 */
    idx_t len = 1 + offset;                    /* len ≥ 1 */

    /* First term: all parts of y have size 1 */
    seq_t result = binom(m - 1, len - 1);

    /* Remaining cases where at least one part of y has size 1 and some
     * parts have size 2 (and y parts ≤2) */
    if (len > 1) {
        for (idx_t k = 2; k <= len - 1; ++k) {
            /* Use signed intermediate to avoid underflow of unsigned ops */
            int64_t m_i  = (int64_t)m;
            int64_t k_i  = (int64_t)k;
            int64_t len_i = (int64_t)len;

            int64_t a1_i = m_i + k_i - len_i - 1;    /* m + k – len – 1 */
            int64_t b1_i = 2 * k_i - len_i - 1;      /* 2k – len – 1 */

            if (a1_i < 0 || b1_i < 0 || a1_i < b1_i) continue;

            seq_t term1 = binom((idx_t)a1_i, (idx_t)b1_i);
            if (term1 == 0) continue;

            int64_t a2_i = k_i;                     /* k */
            int64_t b2_i = 2 * k_i - len_i;         /* 2k – len */

            if (b2_i < 0 || b2_i > a2_i) continue;

            seq_t term2 = binom((idx_t)a2_i, (idx_t)b2_i);
            if (term2 == 0) continue;

            result += term1 * term2;
        }
    }
    return result;
}
