/*
 * A164173  Number of binary strings of length n with equal numbers of 0010
 *          and 1101 substrings.
 * https://oeis.org/A164173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n containing the same
 *            number of the substrings 0010 and 1101. Computed by dynamic
 *            programming over the last 3 bits and the difference d = (#0010)
 *            – (#1101). For n < 4 all strings are valid, so a(n) = 2^n. For n
 *            ≥ 4 the DP iterates over all strings of incr...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* External integer types supplied by the build: seq_t, idx_t */
seq_t a(idx_t n)
{
    /* Base cases: no 4‑bit windows exist */
    if (n < 4) {
        seq_t res = 1;
        for (idx_t i = 0; i < n; ++i)
            res <<= 1;
        return res;
    }

    /* DP arrays: dp_prev and dp_curr store
       dp[ suffix (0..7) ][ diff_offset + d ],
       where d ranges from -max to +max and max = n. */
    size_t size = 2 * (size_t)n + 1;            /* array width of diff axis   */
    size_t offset = n;                          /* index representing diff = 0 */

    seq_t *dp_prev = (seq_t *)calloc(8 * size, sizeof(seq_t));
    seq_t *dp_curr = (seq_t *)calloc(8 * size, sizeof(seq_t));
    if (!dp_prev || !dp_curr)
        abort();

    /* Initialisation: length = 3, diff = 0 for all 3‑bit suffixes */
    for (size_t s = 0; s < 8; ++s)
        dp_prev[s * size + offset] = 1;

    /* Iterate for lengths 4 .. n */
    for (idx_t len = 4; len <= n; ++len) {
        /* zero the current DP table */
        memset(dp_curr, 0, sizeof(seq_t) * 8 * size);

        /* transition */
        for (size_t s = 0; s < 8; ++s) {
            size_t base = s * size;
            for (size_t d = 0; d < size; ++d) {
                seq_t val = dp_prev[base + d];
                if (val == 0)
                    continue;

                /* Append bit 0 */
                size_t ns0 = (s << 1) & 7;
                int pat0 = (int)(s << 1);            /* 4‑bit pattern */
                int delta0 = (pat0 == 2 ? 1 : (pat0 == 13 ? -1 : 0));
                dp_curr[ns0 * size + d + delta0] += val;

                /* Append bit 1 */
                size_t ns1 = ((s << 1) | 1) & 7;
                int pat1 = (int)((s << 1) | 1);
                int delta1 = (pat1 == 2 ? 1 : (pat1 == 13 ? -1 : 0));
                dp_curr[ns1 * size + d + delta1] += val;
            }
        }

        /* If we reached the required length, compute the answer */
        if (len == n) {
            seq_t sum = 0;
            for (size_t s = 0; s < 8; ++s)
                sum += dp_curr[s * size + offset];
            free(dp_prev);
            free(dp_curr);
            return sum;
        }

        /* Prepare for next iteration: swap tables */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* Should never be reached */
    free(dp_prev);
    free(dp_curr);
    return 0;
}
