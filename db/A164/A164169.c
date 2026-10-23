/*
 * A164169  Number of binary strings of length n with equal numbers of 0010
 *          and 1001 substrings.
 * https://oeis.org/A164169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164169:
   a(n) = number of binary strings of length n containing an equal number
   of the substrings 0010 and 1001.
   Computed by dynamic programming over the last three bits of the
   prefix and the current difference d = (#0010) – (#1001).
   For n < 4 no such substrings can appear, so a(n) = 2^n. */

#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n < 4: no occurrences of 0010 or 1001 are possible */
    if (n < 4)
        return pow2(n);   /* 2^n */

    /* stride = 2*n+1 allows indices for differences −n … +n */
    idx_t stride   = 2 * n + 1;
    idx_t zeroOff  = n;          /* index representing difference 0 */

    /* DP tables: 8 states (each 3‑bit suffix) × stride differences */
    seq_t *cur = (seq_t *)calloc(8 * stride, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(8 * stride, sizeof(seq_t));
    if (!cur || !nxt)
        return (seq_t)0;   /* unlikely for realistic n */

    /* Initial state: after 3 bits, each suffix occurs exactly once
       with difference 0. */
    for (idx_t s = 0; s < 8; ++s)
        cur[s * stride + zeroOff] = (seq_t)1;

    int curDiff = 0;   /* current maximum |difference| */

    /* Append bits from position 3 up to n-1 (total n-3 additions) */
    for (idx_t step = 0; step < n - 3; ++step) {
        int nextDiff = curDiff + 1;

        /* Clear the next table */
        memset(nxt, 0, 8 * stride * sizeof(seq_t));

        /* Process all states and all possible differences */
        for (idx_t s = 0; s < 8; ++s) {
            seq_t *src = cur + s * stride;
            for (int d = -curDiff; d <= curDiff; ++d) {
                seq_t val = src[zeroOff + d];
                if (!val)
                    continue;

                /* Append bit 0: new suffix is (s & 3) << 1 */
                {
                    idx_t dest = ((s & 3u) << 1u);
                    int pat   = ((s << 1u) | 0u) & 0xFu;
                    int delta = (pat == 2) ? 1 : 0;   /* 0010 → +1 */
                    int nd    = d + delta;
                    nxt[dest * stride + zeroOff + nd] += val;
                }

                /* Append bit 1: new suffix is ((s & 3) << 1) | 1 */
                {
                    idx_t dest = (((s & 3u) << 1u) | 1u);
                    int pat   = ((s << 1u) | 1u) & 0xFu;
                    int delta = (pat == 9) ? -1 : 0;  /* 1001 → -1 */
                    int nd    = d + delta;
                    nxt[dest * stride + zeroOff + nd] += val;
                }
            }
        }

        /* Prepare for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
        curDiff = nextDiff;
    }

    /* Sum the counts of all suffixes with zero difference */
    seq_t result = (seq_t)0;
    for (idx_t s = 0; s < 8; ++s)
        result += cur[s * stride + zeroOff];

    free(cur);
    free(nxt);
    return result;
}
