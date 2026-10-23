/*
 * A033949  Positive integers that do not have a primitive root.
 * https://oeis.org/A033949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 153, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isOddPrimePower
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = nth positive integer that is NOT of the form p^i or 2p^i,
 * where p is an odd prime (with 1, 2, 4 excluded).
 * The function generates candidates incrementally and checks
 * for exclusion by testing odd prime powers and twice odd prime powers.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isOddPrimePower.h"

/* Main sequence function */
seq_t a(idx_t n) {
    uint64_t target = (uint64_t)n;   /* n up to ~10000 */
    uint64_t count = 0;
    uint64_t candidate = 2;          /* start before first tested number */

    while (count < target) {
        ++candidate;                 /* candidate numbers >= 3 */

        /* Exclusions: 1, 2, 4 */
        if (candidate == 1 || candidate == 2 || candidate == 4) continue;

        /* Exclude odd prime powers p^i (p odd) */
        if (isOddPrimePower(candidate)) continue;

        /* Exclude numbers of the form 2p^i (p odd) */
        if ((candidate & 1ULL) == 0 && (candidate & 3ULL) == 2) {
            uint64_t half = candidate >> 1;      /* candidate / 2 */
            if (isOddPrimePower(half)) continue;
        }

        /* Candidate is part of the sequence */
        ++count;
    }
    return (seq_t)candidate;
}
