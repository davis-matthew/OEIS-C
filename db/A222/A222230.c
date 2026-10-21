/*
 * A222230  In the number n, replace all (decimal) digits '2' with '6' and
 *          vice versa.
 * https://oeis.org/A222230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 199, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implement the mapping that exchanges decimal digits 2 and 6.
   For each decimal digit d of the index n, replace it by
   d' where d' = 0,1,3,4,5,7,8,9 are unchanged, 2 ↔ 6.
   The resulting decimal number is returned. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* mapping table for decimal digits 0–9 */
    static const uint8_t map[10] = {0,1,6,3,4,5,2,7,8,9};

    /* maximum 20 decimal digits for 64‑bit n, plus one extra slot for safety */
    uint8_t rev[21];
    int idx = 0;            /* number of mapped digits collected */

    idx_t x = n;
    /* collect mapped digits from least‑significant to most‑significant */
    while (x > 0) {
        uint8_t d = x % 10;       /* current least‑significant digit */
        rev[idx++] = map[d];      /* store mapped digit */
        x /= 10;
    }

    /* rebuild the number from most‑significant mapped digit to least */
    seq_t result = 0;
    for (int i = idx - 1; i >= 0; --i)
        result = result * 10 + rev[i];

    return result;   /* for n == 0 idx == 0 so result stays 0 */
}
