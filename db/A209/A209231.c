/*
 * A209231  Number of binary words of length n such that there is at least one
 *          0 and every run of consecutive 0's is of length >= 4.
 * https://oeis.org/A209231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 53, u64 n <= 106,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A209231: Number of binary words of length n that contain at least one 0
 *         and every run of consecutive 0's is of length at least 4.
 * Computed by a linear‑time DP with 5 states that enforce the run‑length
 * restriction.  The value a(0)=0, a(1)=0, … as per OEIS.  The function
 * works with any seq_t / idx_t type available in the build. */
seq_t a(idx_t n)
{
    /* dp0 … dp4 hold counts for the 5 automaton states:
       state 0 : last bit 1 or start (no incomplete zero run)
       state 1 : one zero after the last 1 (incomplete run of length 1)
       state 2 : two consecutive zeros (incomplete run of length 2)
       state 3 : three consecutive zeros (incomplete run of length 3)
       state 4 : a zero run of length ≥4 (complete run)                */
    seq_t dp0 = 1, dp1 = 0, dp2 = 0, dp3 = 0, dp4 = 0;
    for (idx_t step = 0; step < n; ++step) {
        seq_t ndp0 = 0, ndp1 = 0, ndp2 = 0, ndp3 = 0, ndp4 = 0;
        /* from state 0 */
        ndp0 += dp0; /* add 1 */
        ndp1 += dp0; /* add 0 -> enter state 1 */
        /* from state 1 */
        ndp2 += dp1; /* add 0 -> state 2 */
        /* from state 2 */
        ndp3 += dp2; /* add 0 -> state 3 */
        /* from state 3 */
        ndp4 += dp3; /* add 0 -> state 4 */
        /* from state 4 (run length ≥4) */
        ndp0 += dp4; /* add 1 -> reset to state 0 */
        ndp4 += dp4; /* add 0 -> stay in state 4 */

        dp0 = ndp0;
        dp1 = ndp1;
        dp2 = ndp2;
        dp3 = ndp3;
        dp4 = ndp4;
    }
    /* All-ones word (no zeros) is counted only in state 0.
       Subtract it to enforce the “at least one 0” condition. */
    seq_t result = dp0 + dp4;
    if (result > 0) result -= 1;
    return result;
}
