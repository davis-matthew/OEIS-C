/*
 * A023670  Convolution of A023533 with itself.
 * https://oeis.org/A023670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} A023533(k) * A023533(n‑k+1) where A023533(t) =
 *            1 if t is a tetrahedral number (t = m(m+1)(m+2)/6 for some
 *            integer m≥1), else 0
 * Range:     u8 n <= 1344, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function iterates over all tetrahedral numbers ≤ n.
   For each such k it checks whether n‑k+1 is also tetrahedral
   by a simple loop (the number of tetrahedral numbers up to n is ≤ 30
   for n ≤ 10 000).  The result is accumulated in seq_t. */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t m = 1;
    for (;; ++m) {
        /* val = m*(m+1)*(m+2)/6  (≤ n because of the following test) */
        seq_t val = (seq_t)m * (m + 1) * (m + 2) / 6;
        if (val > (seq_t)n) break;           /* no more tetrahedral numbers ≤ n */

        /* k is a tetrahedral number */
        idx_t k = (idx_t)val;                /* safe: val ≤ n ≤ 2^64‑1 */
        idx_t j = n - k + 1;                 /* 1 ≤ j ≤ n */

        /* Check if j is a tetrahedral number by iterating over m again */
        idx_t p = 1;
        for (;; ++p) {
            seq_t val2 = (seq_t)p * (p + 1) * (p + 2) / 6;
            if (val2 > (seq_t)j) break;
            if (val2 == (seq_t)j) { result += 1; break; }
        }
    }
    return result;
}
