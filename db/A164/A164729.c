/*
 * A164729  Numbers belonging to cycles of length 7 under the Kaprekar map
 *          A151949.
 * https://oeis.org/A164729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 14, u64 n <= 49, u128 n <= 119,
 *            big any n
 * Helpers:   smallest_of_cycle, sort_sevens
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164729: numbers belonging to 7‑cycles of the decimal Kaprekar map.
   For each even digit length d ≥ 6 there is a unique 7‑cycle.  The smallest
   element of that cycle has the pattern
     4, (m‑1) copies of 3, 2,0,8,7, m copies of 6  where  m = (d‑4)/2.
   The sequence a(n) lists the numbers in each cycle in ascending order,
   cycling through the lengths 6,8,10,…   */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/smallest_of_cycle.h"
#include "../utils/sort_sevens.h"

/* Compute one Kaprekar step: rearrange digits of x in descending order
   to form 'high', rearrange in ascending order (without leading zeros)
   to form 'low', then return high – low. */
static seq_t kaprekar(seq_t x)
{
    /* 4096 digits are sufficient for n up to 10000 (max d ≈ 2856). */
    uint8_t digits[4096];
    size_t len = 0;
    seq_t tmp = x;

    if (tmp == 0) {
        digits[len++] = 0;
    } else {
        while (tmp != 0) {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    int cnt[10] = {0};
    for (size_t i = 0; i < len; ++i)
        ++cnt[digits[i]];

    seq_t high = 0, low = 0;

    /* High: digits sorted descending. */
    for (int d = 9; d >= 0; --d)
        for (int k = 0; k < cnt[d]; ++k)
            high = high * 10 + d;

    /* Low: digits sorted ascending, drop leading zeros. */
    bool leading = true;
    for (int d = 0; d <= 9; ++d)
        for (int k = 0; k < cnt[d]; ++k) {
            if (leading && d == 0)
                continue;
            leading = false;
            low = low * 10 + d;
        }

    return high - low;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                           /* undefined for n < 1 */

    idx_t cycle_index = (n - 1) / 7;         /* 0‑based cycle number */
    idx_t pos_in_cycle = (n - 1) % 7;        /* 0‑based position in the sorted cycle */
    idx_t d = 6 + 2 * cycle_index;           /* digit length of the cycle */

    seq_t start = smallest_of_cycle(d);

    seq_t cycle[7];
    cycle[0] = start;
    for (size_t i = 1; i < 7; ++i)
        cycle[i] = kaprekar(cycle[i - 1]);

    /* Verify that we indeed returned to the start after 7 steps. */
    if (kaprekar(cycle[6]) != start)
        return 0;                           /* error – should not happen */

    sort_sevens(cycle);
    return cycle[pos_in_cycle];
}
