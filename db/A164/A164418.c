/*
 * A164418  Number of binary strings of length n with no substrings equal to
 *          0000 0010 or 0101.
 * https://oeis.org/A164418
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 79, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of binary strings of length n with no substrings 0000, 0010 or 0101.
   The computation uses dynamic programming on the last up to three bits of the
   partial string, avoiding every forbidden pattern when a new bit is appended. */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 4) {
        /* terms below the offset are irrelevant */
        return (seq_t)0;
    }

    /* We encode a state by (len << 3) | mask,
       where len = 0..3 is the number of bits stored in mask (mask < 8). */
    const size_t STATES = 32;            /* 4 possible lengths × 8 mask values */
    seq_t dp[2][STATES];

    /* initialise dp[0] (empty string) */
    for (size_t i = 0; i < STATES; ++i)
        dp[0][i] = (seq_t)0;
    dp[0][0] = (seq_t)1;                 /* empty string: len=0, mask=0 */

    /* iterate over string length */
    for (idx_t step = 0; step < n; ++step) {
        size_t cur  = (size_t)(step & 1);
        size_t next = cur ^ 1;

        /* zero the next layer */
        for (size_t i = 0; i < STATES; ++i)
            dp[next][i] = (seq_t)0;

        /* transition from every existing state */
        for (size_t s = 0; s < STATES; ++s) {
            seq_t val = dp[cur][s];
            if (!val)
                continue;

            unsigned len  = (unsigned)(s >> 3);
            unsigned mask = (unsigned)(s & 7);

            /* add a bit '0' */
            {
                unsigned new_len, new_mask;
                if (len == 3) {
                    unsigned v = (mask << 1) | 0;          /* 4‑bit candidate */
                    if (v == 0 || v == 2 || v == 5)       /* forbidden? */
                        ;                                  /* skip it */
                    else {
                        new_len  = 3;
                        new_mask = v & 7;                   /* last 3 bits */
                        dp[next][new_len << 3 | new_mask] += val;
                    }
                } else {
                    new_len = len + 1;
                    unsigned v = (mask << 1);            /* shift + 0 */
                    new_mask = v & ((1U << new_len) - 1);
                    dp[next][new_len << 3 | new_mask] += val;
                }
            }

            /* add a bit '1' */
            {
                unsigned new_len, new_mask;
                if (len == 3) {
                    unsigned v = (mask << 1) | 1;
                    if (v == 0 || v == 2 || v == 5)
                        ;
                    else {
                        new_len  = 3;
                        new_mask = v & 7;
                        dp[next][new_len << 3 | new_mask] += val;
                    }
                } else {
                    new_len = len + 1;
                    unsigned v = (mask << 1) | 1;
                    new_mask = v & ((1U << new_len) - 1);
                    dp[next][new_len << 3 | new_mask] += val;
                }
            }
        }
    }

    /* sum all terminal states */
    size_t last = (size_t)(n & 1);
    seq_t result = (seq_t)0;
    for (size_t s = 0; s < STATES; ++s)
        result += dp[last][s];

    return result;
}
