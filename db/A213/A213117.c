/*
 * A213117  Number of binary arrays of length n+13 with fewer than 7 ones in
 *          any length 14 subsequence (=less than 50% duty cycle).
 * https://oeis.org/A213117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #binary strings of length n+13 with at most 6 ones in
 *            every consecutive 14‑bit window. The function computes this
 *            value for any n ≥ 1
 * Range:     u8 none, u16 n <= 5, u32 n <= 24, u64 n <= 62, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* The implementation uses a state‑machine over the last 13 bits.  The state set has 2¹³
   elements.  For each state we store the number of accepted strings that end in that
   state.  Transition to a new state is allowed only if the 14‑bit window
   (previous 13 bits + new bit) contains ≤ 6 ones.  We iterate this transition
   n times (starting from 13 arbitrary bits) and finally sum all state counts. */

static seq_t *dp_prev = NULL;
static seq_t *dp_next = NULL;
static const idx_t STATES = 1ULL << 13;   /* 8192 */

seq_t a(idx_t n)
{
    const idx_t MASK = STATES - 1;

    /* Allocate DP tables on first call.  They are reused for subsequent calls. */
    if (!dp_prev) {
        dp_prev = (seq_t*)malloc(STATES * sizeof(seq_t));
        dp_next = (seq_t*)malloc(STATES * sizeof(seq_t));
    }

    /* Init: after length 13, every 13‑bit prefix is allowed once. */
    for (idx_t i = 0; i < STATES; ++i) dp_prev[i] = 1;

    /* Apply the transition n times.  Each iteration corresponds to appending
       one more bit, so after n steps the length is n + 13. */
    for (idx_t step = 0; step < n; ++step) {
        /* Clear the next DP array. */
        for (idx_t i = 0; i < STATES; ++i) dp_next[i] = 0;

        for (idx_t s = 0; s < STATES; ++s) {
            seq_t val = dp_prev[s];
            if (val == 0) continue;

            /* Count ones in the current 13‑bit state. */
            unsigned pc = __builtin_popcount((unsigned)s);

            if (pc > 6) continue;          /* no outgoing transitions */

            idx_t ns = ((s << 1) & MASK);   /* shift left, drop oldest bit */

            /* Adding a 0 is always legal if pc ≤ 6. */
            dp_next[ns] += val;

            /* Adding a 1 is legal only if pc < 6. */
            if (pc < 6) {
                dp_next[ns | 1] += val;
            }
        }

        /* Prepare for the next step. */
        seq_t *tmp = dp_prev; dp_prev = dp_next; dp_next = tmp;
    }

    /* Sum all state counts: that is the number of valid strings of length n+13. */
    seq_t total = 0;
    for (idx_t i = 0; i < STATES; ++i) total += dp_prev[i];

    return total;
}
