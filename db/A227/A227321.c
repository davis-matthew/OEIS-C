/*
 * A227321  a(n) is the least r>=3 such that the difference between the
 *          nearest r-gonal number >= n and n is an r-gonal number.
 * https://oeis.org/A227321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 267, u16 n <= 1999, u32 n <= 1999, u64 n <= 1999,
 *            u128 n <= 1999, big any n
 * Helpers:   pgonal_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A227321 – a(n) = least r ≥ 3 such that the nearest
 * r‑gonal number ≥ n has a difference with n that is again an
 * r‑gonal number.  */
/* The function iterates r = 3,4,…, for each r it finds the
 * minimal m with P_r(m) ≥ n (incremental search), then tests
 * whether the difference d = P_r(m) – n is itself an r‑gonal
 * number by increasing k until P_r(k) ≥ d.  When d == 0 or a
 * matching k is found, r is returned.  */

#include <stdbool.h>
#include "../utils/pgonal_value.h"

seq_t a(idx_t n)
{
    /* Search for the minimal r.  The loop will terminate because
     * the sequence is defined for every n and the test finds a
     * suitable r in finite time.  */
    for (idx_t r = 3; ; ++r)
    {
        /* Find minimal m with P_r(m) ≥ n.  */
        idx_t m = 0;
        while (pgonal_value(r, m) < (seq_t)n)
            ++m;

        seq_t diff = pgonal_value(r, m) - (seq_t)n;

        /* d == 0 means the triangular (or r‑gonal) number equals n.
         * That satisfies the definition.  */
        if (diff == 0)
            return (seq_t)r;

        /* Test whether diff is an r‑gonal number.  Increment k
         * until P_r(k) ≥ diff.  Because k grows only as the
         * square root of diff, this is efficient for the ranges
         * required by the problem.  */
        for (idx_t k = 1;; ++k)
        {
            seq_t val = pgonal_value(r, k);
            if (val == diff)
                return (seq_t)r;
            if (val > diff)
                break;
        }
    }
}
