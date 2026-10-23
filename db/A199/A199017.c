/*
 * A199017  Number of partitions of n into distinct terms of (1,2)-Ulam
 *          sequence, cf. A002858.
 * https://oeis.org/A199017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A199017: number of partitions of n
   into distinct terms of the (1,2)-Ulam sequence A002858.
   The function generates all Ulam numbers ≤ n, then performs a
   standard distinct‑coin change DP to count partitions. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* External definitions */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* A(0) = 1 (empty partition) */
    if (n == 0) {
        seq_t one = 1;
        return one;
    }

    /* ---------- Generate Ulam numbers ≤ n ---------- */
    unsigned short *cnt = (unsigned short *)calloc((size_t)n + 1, sizeof(unsigned short));
    if (!cnt) return 0;                /* memory allocation failed */

    /* Dynamically growable array of Ulam numbers */
    size_t cap = 8;
    idx_t *ulam = (idx_t *)malloc(cap * sizeof(idx_t));
    if (!ulam) { free(cnt); return 0; }

    ulam[0] = 1;
    ulam[1] = 2;
    size_t ulen = 2;

    /* Count the only representation of 1+2=3 */
    if (n >= 3) cnt[3] = 1;

    idx_t last = 2;
    for (;;) {
        /* Find the next candidate with exactly one representation */
        idx_t cand;
        for (cand = last + 1; cand <= n && cnt[cand] != 1; ++cand);
        if (cand > n) break;          /* no more Ulam numbers needed */

        /* Append new Ulam number */
        if (ulen == cap) {
            cap <<= 1;
            ulam = (idx_t *)realloc(ulam, cap * sizeof(idx_t));
            if (!ulam) break;         /* memory error */
        }
        ulam[ulen++] = cand;
        last = cand;

        /* Update representation counts for sums involving the new number */
        for (idx_t i = 0; i < ulen - 1; ++i) {
            idx_t s = ulam[i] + cand;
            if (s <= n) ++cnt[s];
        }
    }
    free(cnt);

    /* ---------- Count partitions into distinct Ulam numbers ---------- */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) { free(ulam); return 0; }
    dp[0] = 1;                     /* one way to partition 0 */

    for (size_t i = 0; i < ulen; ++i) {
        idx_t u = ulam[i];
        if (u > n) break;
        for (idx_t v = n; v >= u; --v) {
            dp[v] += dp[v - u];
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(ulam);
    return result;
}
