/*
 * A043669  Numbers whose base-15 representation has exactly 2 runs.
 * https://oeis.org/A043669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base-15 representation has exactly 2 runs
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset < 1 */

    idx_t cumulative = 0;
    idx_t L = 2;
    /* find the digit length that contains the n-th term */
    for (;;) {
        idx_t countL = 196 * (L - 1);          /* 14 choices for a, 14 for b, L-1 possible run lengths */
        if (cumulative + countL >= n)
            break;
        cumulative += countL;
        ++L;
    }

    idx_t offset = n - cumulative - 1;          /* zero‑based index within numbers of length L */
    idx_t perA = 14 * (L - 1);                  /* numbers per first digit a */

    idx_t a_idx = offset / perA;                /* which first digit */
    uint8_t first = (uint8_t)(a_idx + 1);       /* a is 1..14 */

    idx_t rem = offset % perA;
    idx_t k_idx = rem / 14;                     /* which run length k (1..L-1) */
    uint8_t k = (uint8_t)(k_idx + 1);

    idx_t rem2 = rem % 14;
    uint8_t b = (uint8_t)rem2;                  /* second digit (0..14, skip a) */
    if (b >= first)
        b += 1;                                 /* skip a */

    seq_t val = 0;
    for (uint8_t i = 0; i < k; ++i) {
        val = val * 15 + first;
    }
    for (uint8_t i = k; i < L; ++i) {
        val = val * 15 + b;
    }
    return val;
}
