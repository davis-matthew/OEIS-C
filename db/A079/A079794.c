/*
 * A079794  Smallest number having k digits "k" (k=1,...,n) but any two
 *          adjacent digits are different.
 * https://oeis.org/A079794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest number containing exactly k copies of *
 *            digit k (k = 1 … n) with no two adjacent digits equal. * The
 *            construction is the same as the OEIS description: * 1) Build a
 *            prefix (the *part1*) by repeatedly appending the * smallest
 *            possible digit that is still available and different...
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* total length of the resulting string */
    idx_t total_len = (idx_t)n * ((idx_t)n + 1) / 2;
    /* length of the first part (part1) */
    idx_t part1_len = 0;
    if (n > 1) {
        part1_len = ((idx_t)(n - 1) * (n - 2)) / 2;
    }

    /* buffer for the decimal representation */
    char *buf = (char *)malloc((size_t)total_len + 1);
    if (!buf) return (seq_t)0;
    buf[total_len] = '\0';

    /* counts of remaining occurrences for each digit 1 … n */
    size_t *cnt = (size_t *)calloc((size_t)n + 1, sizeof(size_t));
    if (!cnt) { free(buf); return (seq_t)0; }
    for (size_t k = 1; k <= n; ++k) cnt[k] = k;

    /* Build part1 */
    size_t last_digit = 0;   /* 0 means “no previous digit” */
    for (idx_t pos = 0; pos < part1_len; ++pos) {
        size_t d;
        for (d = 1; d <= n; ++d) {
            if (cnt[d] == 0 || d == last_digit) continue;
            break;   /* use the smallest feasible digit */
        }
        buf[pos] = (char)('0' + (int)d);
        cnt[d]--;           /* consume one copy */
        last_digit = d;
    }

    /* Build part2: start with an n, then interleave the remaining
     * digits (k = 1 … n-1) between the n's in ascending order. */
    size_t idx = part1_len;          /* current position in buf          */
    buf[idx++] = (char)('0' + (int)n);   /* leading n of part2            */

    for (size_t k = 1; k < n; ++k) {
        size_t remain = cnt[k];
        while (remain--) {
            buf[idx++] = (char)('0' + (int)k);
            buf[idx++] = (char)('0' + (int)n);
        }
    }

    /* idx should now equal total_len */
    seq_t result = seq_from_str(buf);
    free(buf);
    free(cnt);
    return result;
}
