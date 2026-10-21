/*
 * A218350  Number of n X 4 binary arrays with every 0 a horizontal or
 *          vertical neighbor to some 1.
 * https://oeis.org/A218350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of n×4 binary arrays in which every 0 has a
 *            horizontal or vertical neighbour equal to 1. It satisfies the
 *            22‑term linear recurrence found by the transfer‑matrix method;
 *            the following DP implements the same recurrence by state
 *            compression, computing a(1)=9, a(2)=149, …
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return a(n) for all n >= 1.  Index type and value type are supplied by the
   build system.  The algorithm uses a 4‑column transfer matrix and runs in
   O(n) time with a very small constant. */
seq_t a(idx_t n)
{
    const unsigned int MASK4 = 0xF;          /* 4 bits for a column */
    static seq_t dp[256];
    static seq_t nxt[256];
    unsigned int bits, zeroMask, verticalSat, unsat, idx, cur, nextIdx;
    unsigned int prevBits, prevUnsat;
    seq_t cnt;

    /* initialise to zero */
    for (int i = 0; i < 256; ++i) dp[i] = (seq_t)0;

    /* first column ---------------------------------------------------- */
    for (bits = 0; bits < 16; ++bits) {
        /* which cells are zero */
        zeroMask   = (~bits) & MASK4;
        /* zeros that have a vertical 1 neighbour */
        verticalSat = ((bits << 1) | (bits >> 1)) & zeroMask;
        /* zeros that are still unsatisfied after considering verticals */
        unsat  = zeroMask & (~verticalSat);
        idx    = bits | (unsat << 4);
        dp[idx] = (seq_t)1;          /* one way to realise this column */
    }

    /* subsequent columns ---------------------------------------------- */
    for (idx_t col = 2; col <= n; ++col) {
        /* clear next array */
        for (int i = 0; i < 256; ++i) nxt[i] = (seq_t)0;

        for (idx = 0; idx < 256; ++idx) {
            cnt = dp[idx];
            if (cnt == (seq_t)0) continue;

            prevBits   = idx & MASK4;
            prevUnsat  = (idx >> 4) & MASK4;

            for (cur = 0; cur < 16; ++cur) {
                /* all unsatisfied zeros in the previous column must be
                   satisfied by a 1 in the current column */
                if ((prevUnsat & (~cur)) != 0) continue;

                zeroMask = (~cur) & MASK4;
                /* zeros satisfied by a horizontal 1 from the left
                   predecessor column */
                unsigned int horizontalSat = prevBits & zeroMask;
                /* zeros satisfied by a vertical 1 in this column */
                verticalSat = ((cur << 1) | (cur >> 1)) & zeroMask;
                unsigned int satisfied = horizontalSat | verticalSat;
                unsat  = zeroMask & (~satisfied);

                nextIdx = cur | (unsat << 4);
                nxt[nextIdx] = nxt[nextIdx] + cnt;
            }
        }

        /* swap dp and nxt */
        for (int i = 0; i < 256; ++i) dp[i] = nxt[i];
    }

    /* sum all states whose unsatisfied mask is zero (every 0 already
       has a neighbour 1) */
    seq_t ans = (seq_t)0;
    for (bits = 0; bits < 16; ++bits)
        ans = ans + dp[bits];

    return ans;
}
