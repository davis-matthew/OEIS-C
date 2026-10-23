/*
 * A162711  Triangle read by rows: T(n,k) = value of the string of length k
 *          beginning at position n in the concatenation of natural numbers in
 *          decimal representation, 1<=k<=n.
 * https://oeis.org/A162711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   a(n) = value of the k‑digit substring of the concatenation of
 *            natural numbers ("123456789101112…") that starts at position r,
 *            where r = smallest integer with r*(r+1)/2 >= n and k = n –
 *            r*(r-1)/2. The implementation generates the required prefix of
 *            the concatenated string, extracts the substring,...
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 76, u64 n <= 229, u128 n <= 779,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, kept safe */

    /* Find r: smallest integer with r*(r+1)/2 >= n */
    idx_t r = 1;
    while (((__uint128_t)r * (r + 1)) / 2 < n) r++;

    /* k = n - r*(r-1)/2 */
    idx_t prev_tri = ((r - 1) * r) / 2;
    idx_t k = n - prev_tri;               /* 1 ≤ k ≤ r */

    /* End position needed in the concatenated string */
    idx_t end_pos = r + k - 1;

    /* Allocate buffer for the concatenated digits up to end_pos */
    char *digits = (char *)malloc((size_t)end_pos + 1);
    if (!digits) return 0;                /* memory failure */

    size_t pos = 0;                       /* current write position (0‑based) */
    idx_t num = 1;                        /* number being written */

    while (pos < (size_t)end_pos) {
        /* Convert current number to decimal string */
        char numbuf[32];
        int len = snprintf(numbuf, sizeof(numbuf), "%jd", (intmax_t)num);
        if (len <= 0) { free(digits); return 0; }

        /* Copy as many digits as needed without overflowing */
        size_t copy = len;
        if (pos + copy > (size_t)end_pos) copy = (size_t)end_pos - pos;
        memcpy(digits + pos, numbuf, copy);
        pos += copy;
        num++;
    }
    digits[end_pos] = '\0';                 /* ensure null‑termination */

    /* Extract the desired substring (1‑based position r) */
    size_t start = r - 1;                  /* convert to 0‑based index */
    if (start + k > end_pos) { free(digits); return 0; }

    char *substr = (char *)malloc((size_t)k + 1);
    if (!substr) { free(digits); return 0; }

    memcpy(substr, digits + start, k);
    substr[k] = '\0';

    seq_t result = seq_from_str(substr);

    free(substr);
    free(digits);
    return result;
}
