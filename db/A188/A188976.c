/*
 * A188976  Number of nX2 array permutations with each element moved but moved
 *          no more than a city block distance of two.
 * https://oeis.org/A188976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   seq_realloc_wrapper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A188976
 *  Recurrence (order 20):
 *      a(n) =  8*a(n-1) + 3*a(n-2) + 68*a(n-3) - 142*a(n-4) - 354*a(n-5)
 *              +225*a(n-6) +430*a(n-7) +47*a(n-8) +308*a(n-9)
 *              -16*a(n-10) -932*a(n-11) +281*a(n-12) +24*a(n-13)
 *              -195*a(n-14) +112*a(n-15) +22*a(n-16) +22*a(n-17)
 *              -a(n-18) -6*a(n-19) +a(n-20)
 *  Offset: 1 (a(1) is the first term given below)
 *  Initial terms (for n = 1 … 19):
 *      a(1)=1, a(2)=9, a(3)=116, a(4)=900, a(5)=7836,
 *      a(6)=71865, a(7)=640513, a(8)=5706113, a(9)=51056136,
 *      a(10)=456396688, a(11)=4078122040, a(12)=36448590305,
 *      a(13)=325763780945, a(14)=2911451082361, a(15)=26020738951500,
 *      a(16)=232557947597540, a(17)=2078461123504804,
 *      a(18)=18576017680577225, a(19)=166021162669244641
 */

#include <stdlib.h>
#include "../utils/seq_realloc_wrapper.h"

#ifdef __cplusplus

#else

#endif

/* Memoisation table – static across calls. */
static seq_t *memo = NULL;
static idx_t memo_len = 0;   /* one past the largest computed index */
static size_t memo_cap = 0;  /* allocated capacity */

/* Coefficients: a(n-1) … a(n-20). */
static const int coeffs[20] = {
    8,   3,   68, -142, -354, 225, 430,  47, 308,
  -16, -932, 281,  24, -195, 112,  22,  22, -1, -6, 1
};

/* Make sure memo table contains values up to index n. */
static void ensure_memo(idx_t n) {
    if (n < memo_len) return;          /* already computed */

    /* Expand capacity if needed. */
    if (memo_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *newmem = seq_realloc_wrapper(memo, memo_cap, new_cap);
        memo = newmem;
        memo_cap = new_cap;
    }

    /* Initialise base values on first run. */
    if (memo_len == 0) {
        /* Ensure capacity for at least 20 initial values. */
        if (memo_cap < 20) {
            size_t new_cap = 20;
            seq_t *newmem = seq_realloc_wrapper(memo, memo_cap, new_cap);
            memo = newmem;
            memo_cap = new_cap;
        }

        /* a(0) is required by the recurrence; OEIS implicit value is 1. */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)1;
        memo[2] = (seq_t)9;
        memo[3] = (seq_t)116;
        memo[4] = (seq_t)900;
        memo[5] = (seq_t)7836;
        memo[6] = (seq_t)71865;
        memo[7] = (seq_t)640513;
        memo[8] = (seq_t)5706113;
        memo[9] = (seq_t)51056136;
        memo[10] = (seq_t)456396688;
        memo[11] = (seq_t)4078122040;
        memo[12] = (seq_t)36448590305;
        memo[13] = (seq_t)325763780945;
        memo[14] = (seq_t)2911451082361;
        memo[15] = (seq_t)26020738951500;
        memo[16] = (seq_t)232557947597540;
        memo[17] = (seq_t)2078461123504804;
        memo[18] = (seq_t)18576017680577225;
        memo[19] = (seq_t)166021162669244641;
        memo_len = 20;          /* memo[0] … memo[19] are ready */
    }

    /* Compute higher indices using the recurrence. */
    for (idx_t idx = memo_len; idx <= n; ++idx) {
        if (idx < 20) continue;   /* already initialised */

        seq_t pos = 0;  /* sum of positive‑coefficient terms */
        seq_t neg = 0;  /* sum of absolute values of negative‑coefficient terms */

        for (int j = 0; j < 20; ++j) {
            seq_t v = memo[idx - 1 - j];
            int c = coeffs[j];
            if (c > 0) {
                pos += v * (seq_t)c;
            } else {
                neg += v * (seq_t)(-c);
            }
        }
        memo[idx] = pos - neg;   /* pos >= neg for this recurrence */
    }
    memo_len = n + 1;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return 0;   /* sequence is defined for n≥1 */
    ensure_memo(n);
    return memo[n];
}
