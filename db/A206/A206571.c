/*
 * A206571  Number of nX2 0..3 arrays avoiding the pattern z-1 z-1 z in any
 *          row, column or nw-to-se diagonal.
 * https://oeis.org/A206571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (f(n))², where f(n) = number of n‑length sequences over
 *            {0,1,2,3} that avoid the forbidden patterns 0 0 1, 1 1 2, 2 2 3.
 *            f(1) = 4 → a(1) = 16 f(2) = 16 → a(2) = 256 For n ≥ 3 compute
 *            f(n) by a DFA on the last two symbols
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 207 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t (not really used here but harmless) */

static inline seq_t
compute_f_n(idx_t n)
{
    /* f(1) */
    if (n == (idx_t)1)
        return (seq_t)4;
    /* f(2) */
    if (n == (idx_t)2)
        return (seq_t)16;

    /* States are pairs (a,b), 0 ≤ a,b ≤ 3, encoded as a*4+b (0…15). */
    seq_t cur[16];
    seq_t nxt[16];
    /* Initial counts for length 2: one way for each of the 16 states. */
    for (int s = 0; s < 16; ++s)
        cur[s] = (seq_t)1;

    /* Iterate from length 3 up to n. */
    for (idx_t len = 3; len <= n; ++len) {
        /* zero next array */
        for (int s = 0; s < 16; ++s)
            nxt[s] = (seq_t)0;
        /* transition */
        for (int s = 0; s < 16; ++s) {
            seq_t ways = cur[s];
            if (ways == (seq_t)0)
                continue;
            int a = s / 4;
            int b = s % 4;
            /* try all possible next symbols c */
            for (int c = 0; c <= 3; ++c) {
                /* skip forbidden patterns */
                if (a == 0 && b == 0 && c == 1)      continue;
                if (a == 1 && b == 1 && c == 2)      continue;
                if (a == 2 && b == 2 && c == 3)      continue;
                int ns = (b << 2) | c;   /* new state (b,c) */
                nxt[ns] += ways;
            }
        }
        /* copy next to current for next iteration */
        for (int s = 0; s < 16; ++s)
            cur[s] = nxt[s];
    }

    /* sum all states to obtain f(n) */
    seq_t result = (seq_t)0;
    for (int s = 0; s < 16; ++s)
        result += cur[s];
    return result;
}

seq_t
a(idx_t n)
{
    seq_t fn = compute_f_n(n);
    return fn * fn;
}
