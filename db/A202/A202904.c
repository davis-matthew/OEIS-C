/*
 * A202904  Number of n X 6 0..1 arrays with every one equal to some NW, E or
 *          S neighbor.
 * https://oeis.org/A202904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n X 6 0..1 arrays with every one equal to some * NW, E
 *            or S neighbor. * (Implementation uses dynamic programming over
 *            6-bit masks.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* State index: (prevMask << 6) | needMask   (prevMask, needMask ∈ [0,63]) */
#define W 6
#define MASK_ALL 0x3F          /* 0b111111 */
#define STATE_COUNT 4096

/* pre‑computed transition information */
static uint8_t  supCount[STATE_COUNT];
static uint16_t supState[STATE_COUNT][64];
static int      precomputed = 0;

/* Build transition tables once */
static void init_tables(void)
{
    if (precomputed) return;
    precomputed = 1;

    for (int oldMask = 0; oldMask < (1 << W); ++oldMask) {
        int NWMask = (oldMask << 1) & MASK_ALL;
        for (int needMask = 0; needMask < (1 << W); ++needMask) {
            int stateIdx   = (oldMask << W) | needMask;
            int complement = (~needMask) & MASK_ALL;
            int sub = complement;
            unsigned char cnt = 0;
            while (1) {
                int newMask = needMask | sub;
                int EastMask = newMask >> 1;
                int satisfied = NWMask | EastMask;
                int newNeedMask = newMask & (~satisfied) & MASK_ALL;
                int newStateIdx = (newMask << W) | newNeedMask;
                supState[stateIdx][cnt++] = (uint16_t)newStateIdx;
                if (sub == 0) break;
                sub = (sub - 1) & complement;
            }
            supCount[stateIdx] = cnt;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1; behaviour for n<1 irrelevant */

    init_tables();

    /* Allocate two DP arrays of size STATE_COUNT */
    seq_t *dp     = (seq_t*)calloc(STATE_COUNT, sizeof(seq_t));
    seq_t *nextdp = (seq_t*)calloc(STATE_COUNT, sizeof(seq_t));
    if (!dp || !nextdp) return 0;           /* allocation failure fallback */

    dp[0] = 1;  /* state (prevMask=0, needMask=0) */

    /* Process each row */
    for (idx_t i = 0; i < n; ++i) {
        /* zero nextdp */
        memset(nextdp, 0, STATE_COUNT * sizeof(seq_t));

        for (int state = 0; state < STATE_COUNT; ++state) {
            seq_t cnt = dp[state];
            if (cnt == 0) continue;

            unsigned char transitions = supCount[state];
            uint16_t *outStates = supState[state];
            for (unsigned char k = 0; k < transitions; ++k) {
                nextdp[outStates[k]] += cnt;
            }
        }
        /* swap dp and nextdp */
        seq_t *tmp = dp;
        dp = nextdp;
        nextdp = tmp;
    }

    /* Sum over all states where the last row's needMask is zero */
    seq_t result = 0;
    for (int prevMask = 0; prevMask < (1 << W); ++prevMask) {
        int idx = (prevMask << W) | 0;
        result += dp[idx];
    }

    /* free temporary arrays */
    free(dp);
    free(nextdp);

    return result;
}
