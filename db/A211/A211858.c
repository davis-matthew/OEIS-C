/*
 * A211858  Number of partitions of n into parts <= 3 with the property that
 *          all parts have distinct multiplicities.
 * https://oeis.org/A211858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts <= 3 such that all
 *            non‑zero multiplicities are distinct
 * Range:     u8 n <= 54, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t cnt = 0;
    for (idx_t m3 = 0; m3 <= n / 3; ++m3) {
        idx_t rem3 = n - 3 * m3;
        for (idx_t m2 = 0; m2 <= rem3 / 2; ++m2) {
            idx_t m1 = rem3 - 2 * m2;
            bool ok = true;

            /* check pairwise distinctness among the non‑zero multiplicities */
            if (m1 > 0 && m2 > 0 && m3 > 0) {
                if (m1 == m2 || m1 == m3 || m2 == m3) ok = false;
            } else {
                if (m1 > 0 && m2 > 0 && m1 == m2) ok = false;
                if (m1 > 0 && m3 > 0 && m1 == m3) ok = false;
                if (m2 > 0 && m3 > 0 && m2 == m3) ok = false;
            }

            if (ok) ++cnt;
        }
    }
    return cnt;
}
