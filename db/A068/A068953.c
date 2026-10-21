/*
 * A068953  Number of bases B (2 <= B <= n) such that every digit of n in base
 *          B is 0 or 1.
 * https://oeis.org/A068953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of bases B (2 ≤ B ≤ n) such that the
 *            representation of n in base B has only digits 0 or 1. The
 *            computation uses the observation that for B > √n the only
 *            admissible bases are B = n and B = n − 1. For B ≤ √n each base
 *            is tested directly
 * Range:     u8 n <= 1617, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   base_has_only_01
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/base_has_only_01.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    if (n >= 2) {
        /* B = n is always valid (digits 1,0) */
        count++;

        /* B = n‑1 is valid for n > 2 (digits 1,1) */
        if (n > 2) count++;

        /* test bases up to ⌊√n⌋ */
        idx_t lim = 0;
        /* compute floor(sqrt(n)) using 128‑bit arithmetic to avoid overflow */
        while ((unsigned __int128)(lim + 1) * (unsigned __int128)(lim + 1) <= (unsigned __int128)n)
            lim++;

        for (idx_t b = 2; b <= lim; ++b) {
            if (base_has_only_01((seq_t)n, b))
                count++;
        }
    }

    /* for n == 1, count remains 0 as no base ≥ 2 exists */
    return count;
}
