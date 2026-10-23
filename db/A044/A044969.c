/*
 * A044969  Numbers whose base-3 representation contains no 0's and exactly
 *          four 1's.
 * https://oeis.org/A044969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th smallest positive integer whose base‑3
 *            expansion * uses only digits 1 and 2 and contains exactly four
 *            1’s
 * Range:     u8 n <= 6, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   choose_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/choose_fixed.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset is 1; n=0 is undefined */
    uint64_t nn = (uint64_t)n;

    /* Find the base‑3 length L such that the cumulative count of numbers
       with lengths < L is < n <= cumulative count with length L. */
    uint64_t cumulative = 0;
    uint64_t len;
    for (len = 4; ; ++len) {
        uint64_t cnt = choose_fixed(len, 4);
        if (cumulative + cnt >= nn) break;
        cumulative += cnt;
    }

    /* rank (0‑based) of the desired combination among all length‑len
       numbers. */
    uint64_t rank = nn - cumulative - 1;

    /* Unrank the combination of 4 positions among len digits. */
    uint64_t pos[4];
    uint64_t start = 0;
    for (int t = 0; t < 4; ++t) {
        for (uint64_t x = start; x <= len - (4 - t); ++x) {
            uint64_t remaining = len - x - 1;
            uint64_t need    = 4 - t - 1;
            uint64_t cnt     = choose_fixed(remaining, need);
            if (rank < cnt) {
                pos[t] = x;
                start = x + 1;
                break;
            }
            rank -= cnt;
        }
    }

    /* Compute the base‑10 value of the base‑3 number from the chosen
       positions of the digit 1. */
    seq_t val = 0;
    for (uint64_t i = 0; i < len; ++i) {
        uint64_t digit = 2;
        if (i == pos[0] || i == pos[1] || i == pos[2] || i == pos[3])
            digit = 1;
        val = val * 3 + digit;
    }
    return val;
}
