/*
 * A207889  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   counts 7×n binary arrays avoiding horizontal patterns 000,101 *
 *            and vertical patterns 011,110 by transfer‑matrix DP with 35
 *            states
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 31, u128 n <= 65,
 *            big any n
 * Helpers:   vertical_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/vertical_ok.h"

seq_t a(idx_t n) {
    /* ---------- static pre‑computation of column states and transitions ---------- */
    static bool initialized = false;
    static size_t S = 0;          /* number of vertical‑legal columns */
    static int *col = NULL;       /* col[i] is the 7‑bit mask of state i   */
    static uint64_t **nextMask = NULL;   /* nextMask[p][q] – bitmask of allowed r */

    if (!initialized) {
        /* enumerate vertical‑legal columns */
        int tmp[128];
        size_t cnt = 0;
        for (int m = 0; m < 128; ++m)
            if (vertical_ok(m))
                tmp[cnt++] = m;
        S = cnt;                          /* S == 35 for 7 rows */
        col = (int*)malloc(S * sizeof(int));
        for (size_t i = 0; i < S; ++i) col[i] = tmp[i];

        /* pre‑compute transition masks */
        nextMask = (uint64_t**)malloc(S * sizeof(uint64_t*));
        for (size_t i = 0; i < S; ++i) {
            nextMask[i] = (uint64_t*)malloc(S * sizeof(uint64_t));
            for (size_t j = 0; j < S; ++j) {
                uint64_t msk = 0;
                for (size_t k = 0; k < S; ++k) {
                    bool ok = true;
                    for (int bit = 0; bit < 7; ++bit) {
                        int a = (col[i] >> bit) & 1;
                        int b = (col[j] >> bit) & 1;
                        int c = (col[k] >> bit) & 1;
                        if ((a == 0 && b == 0 && c == 0) ||
                            (a == 1 && b == 0 && c == 1)) {
                            ok = false; break;
                        }
                    }
                    if (ok) msk |= (1ULL << k);
                }
                nextMask[i][j] = msk;
            }
        }
        initialized = true;
    }

    /* ---------- handle small n ---------- */
    if (n < 1) return 0;
    if (n == 1) return (seq_t)S;                   /* all vertical‑legal columns   */
    if (n == 2) return (seq_t)S * (seq_t)S;           /* all pairs, no triple yet    */

    /* ---------- DP for n >= 3 ---------- */
    const size_t pairs = S * S;
    seq_t *dp  = (seq_t*)calloc(pairs, sizeof(seq_t));
    seq_t *tmp = NULL;

    /* start with all pairs having count 1 */
    for (size_t idx = 0; idx < pairs; ++idx) dp[idx] = 1;

    /* iterate columns 3 … n */
    for (size_t step = 3; step <= (size_t)n; ++step) {
        tmp = (seq_t*)calloc(pairs, sizeof(seq_t));
        for (size_t p = 0; p < S; ++p) {
            for (size_t q = 0; q < S; ++q) {
                seq_t cnt = dp[p * S + q];
                if (cnt == 0) continue;
                uint64_t msk = nextMask[p][q];
                while (msk) {
                    unsigned r = __builtin_ctzll(msk);
                    msk &= msk - 1;
                    tmp[q * S + r] += cnt;
                }
            }
        }
        free(dp);
        dp = tmp;
    }

    /* sum all states to get total count */
    seq_t total = 0;
    for (size_t idx = 0; idx < pairs; ++idx) {
        total += dp[idx];
    }
    free(dp);
    return total;
}
