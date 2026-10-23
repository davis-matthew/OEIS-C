/*
 * A046828  Digits of n^2 include digits of n, n does not end in 0.
 * https://oeis.org/A046828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046828: the n‑th positive integer n (n≠0, never ending in 0) for which
   every digit of n occurs in n² at least as many times as it occurs in n. */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Found count of qualifying numbers so far. */
    idx_t found = 0;
    /* Candidate integer to test. */
    idx_t cand = 1;

    for (;; ++cand) {
        /* Skip numbers ending in 0. */
        if (cand % 10 == 0) continue;

        /* Count digits of cand. */
        unsigned int need[10] = {0};
        idx_t tmp = cand;
        while (tmp > 0) {
            need[(unsigned int)(tmp % 10)]++;
            tmp /= 10;
        }

        /* Compute cand² as seq_t. */
        seq_t sq = (seq_t)cand;
        sq = sq * sq;

        /* Count digits of square. */
        unsigned int have[10] = {0};
        seq_t sq_tmp = sq;
        while (sq_tmp > 0) {
            have[(unsigned int)(sq_tmp % 10)]++;
            sq_tmp /= 10;
        }

        /* Verify multiplicity condition. */
        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if (need[d] > have[d]) {
                ok = false;
                break;
            }
        }

        if (ok) {
            ++found;
            if (found == n) return (seq_t)cand;
        }
    }
}
