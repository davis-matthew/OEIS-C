/*
 * A023708  Numbers with exactly 3 0's in their base 4 expansion.
 * https://oeis.org/A023708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow3_capped
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with exactly 3 zero digits in base‑4 representation. */
/* The sequence is the list of all positive integers whose base‑4
 * expansion contains exactly three occurrences of the digit 0.
 *
 * The value a(n) is the n‑th smallest such integer (a(1) = 64).
 * This implementation computes a(n) without hard‑coding the
 * terms.  It determines the length L of the base‑4 representation
 * that contains the n‑th number and then un‑ranks that number
 * using combinatorial counting (choosing the digits greedily).
 */

#include <stdint.h>
#include "../utils/pow3_capped.h"

static inline idx_t comb_small(idx_t n, uint8_t k)
{
    /* Compute C(n,k) where k <= 3.  Use 128‑bit arithmetic to avoid
     * overflow on 64‑bit compilers. */
    if (k > n) return 0;
    if (k == 0) return 1;
    if (k == 1) return n;
    if (k == 2) return (idx_t)((__int128)n * (n - 1) / 2);
    /* k == 3 */
    return (idx_t)((__int128)n * (n - 1) * (n - 2) / 6);
}

static inline idx_t suffix_count(idx_t remP, idx_t remZ)
{
    /* Number of suffixes of length remP that contain exactly remZ zeros. */
    idx_t comb = comb_small(remP, remZ);
    idx_t pow3 = pow3_capped(remP - remZ);
    __int128 mult = (__int128)comb * pow3;
    return (mult > (__int128)UINT64_MAX) ? UINT64_MAX : (idx_t)mult;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; value for 0 is undefined */

    /* Find the length L of the base‑4 representation that contains a(n). */
    idx_t cum = 0;          /* cumulative count of numbers with < L digits */
    idx_t L = 4;            /* minimal length (need at least 4 digits for 3 zeros) */
    idx_t count = 0;

    for (;;) {
        /* Count of numbers of length L */
        idx_t comb = comb_small(L - 1, 3);          /* C(L-1,3) */
        idx_t pow3 = pow3_capped(L - 3);           /* 3^(L-3) */
        __int128 mult = (__int128)comb * pow3;
        count = (mult > (__int128)UINT64_MAX) ? UINT64_MAX : (idx_t)mult;

        __int128 total = (__int128)cum + count;
        if (total >= n) break;    /* n lies within this length */
        cum = (idx_t)total;
        ++L;
    }

    /* Local index within numbers of length L (0‑based). */
    idx_t local = n - cum - 1;
    idx_t remainingZeros = 3;
    idx_t remainingPos   = L;
    seq_t result = 0;

    for (idx_t pos = 0; pos < L; ++pos) {
        /* Try each possible digit in ascending order. */
        for (uint8_t d = (pos == 0 ? 1 : 0); d <= 3; ++d) {
            idx_t usedZero = (d == 0 ? 1 : 0);
            if (remainingZeros < usedZero) continue;

            idx_t remZ = remainingZeros - usedZero;
            idx_t remP = remainingPos - 1;

            idx_t suffix = suffix_count(remP, remZ);

            if (local < suffix) {
                /* Pick this digit. */
                result = result * 4 + (seq_t)d;
                remainingZeros = remZ;
                remainingPos = remP;
                break;
            } else {
                local -= suffix;
            }
        }
    }
    return result;
}
