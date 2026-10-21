/*
 * A223986  Number of nX7 0..3 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A223986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 7 arrays over {0,1,2,3} with each row
 *            unimodal and each column nondecreasing. The computation is
 *            performed by dynamic programming over all 4¹⁰⁴ = 16384 states
 *            (the 7‑tuples of digits 0‑3)
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 20, u128 n <= 191,
 *            big any n
 * Helpers:   is_unimodal_vector
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_unimodal_vector.h"

/* Maximum number of states: 4^7 = 16384 */
#define N_STATES (1u << 14)   /* 2^(2*7) */

/* Precomputed flag for whether a state is a unimodal vector */
static bool unimodal[N_STATES] = {0};

/* Ensure the unimodal table is initialized once. */
static void init_unimodal_table(void) {
    static bool done = false;
    if (done) return;
    for (uint32_t i = 0; i < N_STATES; ++i)
        unimodal[i] = is_unimodal_vector(i);
    done = true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    init_unimodal_table();

    static seq_t dp[N_STATES];
    static seq_t next[N_STATES];
    memset(dp, 0, sizeof(dp));
    dp[0] = 1;                     /* state of all zeros */

    for (idx_t step = 0; step < n; ++step) {
        /* Copy current DP to next (will be transformed). */
        memcpy(next, dp, sizeof(dp));

        /* Prefix sums along each dimension (base‑4). */
        for (int d = 0; d < 7; ++d) {
            uint32_t stride = 1u << (2 * d);   /* 4^d */
            for (uint32_t i = 0; i < N_STATES; ++i) {
                if ((i >> (2 * d)) & 3u) {     /* digit > 0 */
                    next[i] += next[i - stride];
                }
            }
        }

        /* Keep only unimodal states (others become 0). */
        for (uint32_t i = 0; i < N_STATES; ++i)
            if (!unimodal[i]) next[i] = 0;

        /* Prepare for next iteration. */
        memcpy(dp, next, sizeof(dp));
    }

    /* Sum over all unimodal states. */
    seq_t total = 0;
    for (uint32_t i = 0; i < N_STATES; ++i) {
        if (unimodal[i])
            total += dp[i];
    }
    return total;
}
