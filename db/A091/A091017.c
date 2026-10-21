/*
 * A091017  Nonpalindromic integers which have an even number of ones in
 *          binary and whose reverse does as well.
 * https://oeis.org/A091017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th non‑palindromic integer that has an even number
 *            of ones in its binary representation and whose decimal reverse
 *            also has an even number of ones. Computed by brute‑force search
 * Range:     u8 n <= 53, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   even_popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/even_popcount.h"

seq_t a(idx_t n)
{
    uint64_t cand = 0;      /* candidate integer (works up to 64‑bit) */
    idx_t    found = 0;    /* how many valid terms seen */

    while (found < n) {
        ++cand;            /* next integer */

        /* compute decimal reverse and check palindrome at the same time */
        uint64_t t = cand, rev = 0;
        while (t > 0) {
            rev = rev * 10 + t % 10;
            t /= 10;
        }
        if (cand == rev)  /* skip palindromic numbers */
            continue;

        if (!even_popcount(cand))
            continue;     /* must have even number of ones */

        if (!even_popcount(rev))
            continue;     /* reverse must also have even number of ones */

        ++found;          /* found a valid term */
    }

    return (seq_t)cand;    /* cast to the required output type */
}
