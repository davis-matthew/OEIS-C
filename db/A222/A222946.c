/*
 * A222946  Triangle for hypotenuses of primitive Pythagorean triangles.
 * https://oeis.org/A222946
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl, look
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 7261, u32 n <= 7261, u64 n <= 7261,
 *            u128 n <= 7261, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: returns the n-th value of OEIS A222946.
   a(n) is the entry that appears row‑wise in the triangle
   a(r,m) = r^2 + m^2 if r>m≥1, gcd(r,m)=1 and r,m have opposite
   parity, otherwise 0.  The offset is 2; that is, a(2) is the first
   non‑zero entry (5). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;          /* undefined for n<2 */

    idx_t idx = n - 1;                    /* 1‑based position in row‑wise enumeration */

    /* Find the row r such that the cumulative number of entries
       up to row r is at least idx.  Row r has r‑1 entries. */
    idx_t r = 2;
    while ((r - 1) * r / 2 < idx) {
        ++r;
    }

    /* Position m within row r (1‑based).  m ranges from 1 to r‑1. */
    idx_t prev_sum = (r - 2) * (r - 1) / 2;
    idx_t m = idx - prev_sum;              /* m ∈ [1, r‑1] */

    /* Check coprimality and opposite parity. */
    seq_t r_se = (seq_t)r;
    seq_t m_se = (seq_t)m;
    if (gcd(r_se, m_se) == (seq_t)1 && ((r ^ m) & 1)) {
        return r_se * r_se + m_se * m_se;
    } else {
        return (seq_t)0;
    }
}
