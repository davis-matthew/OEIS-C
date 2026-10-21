/*
 * A081355  Levenshtein distance between n and n^2 in decimal representation.
 * https://oeis.org/A081355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Levenshtein distance between the decimal representations
 *            of n and n*n (edit distance with insert/delete/substitute cost
 *            1)
 * Range:     u8 n <= 16, u16 n <= 523, u32 n <= 9535, u64 n <= 9535,
 *            u128 n <= 9535, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    /* Maximum decimal digits for a 64‑bit number squared is 39,
       so 128 gives ample headroom for larger seq_t types. */
    uint8_t digits_n[128];
    uint8_t digits_sq[128];
    size_t len_n, len_sq;

    get_digits((seq_t)n, digits_n, &len_n);
    get_digits((seq_t)n * (seq_t)n, digits_sq, &len_sq);

    int prev[128];
    int curr[128];
    size_t j;

    for (j = 0; j <= len_sq; ++j)
        prev[j] = (int)j;          /* distance from empty prefix of n to first j digits of n^2 */

    for (size_t i = 1; i <= len_n; ++i) {
        curr[0] = (int)i;          /* distance from first i digits of n to empty prefix of n^2 */
        for (j = 1; j <= len_sq; ++j) {
            int cost = (digits_n[i-1] == digits_sq[j-1]) ? 0 : 1;
            int del = prev[j] + 1;
            int ins = curr[j-1] + 1;
            int sub = prev[j-1] + cost;
            int m = (del < ins) ? del : ins;
            curr[j] = (m < sub) ? m : sub;
        }
        for (j = 0; j <= len_sq; ++j)
            prev[j] = curr[j];
    }

    return (seq_t)prev[len_sq];
}
