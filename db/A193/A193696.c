/*
 * A193696  Number of arrays of -2..2 integers x(1..n) with every x(i) in a
 *          subsequence of length 1, 2 or 3 with sum zero.
 * https://oeis.org/A193696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(2) = 5 a(3) = 23 a(n) = number of arrays x(1..n) of
 *            integers in [-2,2] such that each position i is contained in at
 *            least one contiguous subarray of length 1, 2, or 3 whose sum is
 *            zero
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define STATE_VALS 5          /* values -2..2 mapped to 0..4 */
#define STATE_BITS 2          /* pending flag, existence flag */
#define STATE_COUNT (STATE_VALS * STATE_BITS * STATE_BITS * STATE_VALS * STATE_BITS * STATE_BITS)

/* Encode state components into a single index.
   Order of components: v1,p1,e1,v0,p0,e0   */
static inline int encode(int v1, int p1, int e1, int v0, int p0, int e0)
{
    return (((( (v1)*2 + p1)*2 + e1)*5 + v0)*2 + p0)*2 + e0;
}

/* Return the number of valid arrays of length n.
   The function works for any seq_t and idx_t types provided by the build. */
seq_t a(idx_t n)
{
    seq_t dp[STATE_COUNT]   = {0};
    seq_t next[STATE_COUNT] = {0};

    /* initial state: no positions exist yet */
    int init_idx = encode(0,0,0,0,0,0);
    dp[init_idx] = 1;

    for (idx_t step = 0; step < n; ++step) {
        /* clear next array */
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;

        /* iterate over all possible states */
        for (int v1 = 0; v1 < STATE_VALS; ++v1)
        for (int p1 = 0; p1 < STATE_BITS; ++p1)
        for (int e1 = 0; e1 < STATE_BITS; ++e1)
        for (int v0 = 0; v0 < STATE_VALS; ++v0)
        for (int p0 = 0; p0 < STATE_BITS; ++p0)
        for (int e0 = 0; e0 < STATE_BITS; ++e0) {
            int idx = encode(v1,p1,e1,v0,p0,e0);
            seq_t cur = dp[idx];
            if (!cur) continue;

            /* integer values in range -2..2 */
            int val1 = v1 - 2;
            int val0 = v0 - 2;

            /* try each possible new value vnew (0..4) */
            for (int vnew = 0; vnew < STATE_VALS; ++vnew) {
                int val_new = vnew - 2;

                /* pending status for new element: 0 if covered immediately */
                int new_pending = (val_new == 0) ? 0 : 1;

                int pending1 = p1;   /* pending status of old last element */
                int pending0 = p0;   /* pending status of element two steps back */

                /* Length‑2 zero‑sum subarray check (requires e1==1) */
                if (e1 && (val1 + val_new == 0)) {
                    pending1 = 0;
                    new_pending = 0;
                }

                /* Length‑3 zero‑sum subarray check (requires e1==1 && e0==1) */
                if (e1 && e0 && (val0 + val1 + val_new == 0)) {
                    pending0 = 0;
                    pending1 = 0;
                    new_pending = 0;
                }

                /* Position two steps back must now be covered */
                if (pending0) continue;   /* invalid transition */

                /* Construct next state after shifting positions */
                int new_e1 = 1;          /* current element becomes new last */
                int new_e0 = e1;        /* old last becomes second last */
                int new_v1 = vnew;      /* value of new last */
                int new_p1 = new_pending;   /* pending of new last */
                int new_v0 = v1;        /* value of old last (now second) */
                int new_p0 = pending1;     /* pending of old last after update */

                int new_idx = encode(new_v1,new_p1,new_e1,new_v0,new_p0,new_e0);
                next[new_idx] += cur;
            }
        }

        /* copy next into dp for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i) dp[i] = next[i];
    }

    /* Sum over terminal states where all existing positions are covered */
    seq_t result = 0;
    for (int v1 = 0; v1 < STATE_VALS; ++v1)
    for (int p1 = 0; p1 < STATE_BITS; ++p1)
    for (int e1 = 0; e1 < STATE_BITS; ++e1)
    for (int v0 = 0; v0 < STATE_VALS; ++v0)
    for (int p0 = 0; p0 < STATE_BITS; ++p0)
    for (int e0 = 0; e0 < STATE_BITS; ++e0) {
        if (!e1) continue;             /* must have at least one element */

        if (p1) continue;              /* last position must be covered */
        if (e0 && p0) continue;        /* second last must be covered if present */

        int idx = encode(v1,p1,e1,v0,p0,e0);
        result += dp[idx];
    }
    return result;
}
