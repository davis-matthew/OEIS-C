/*
 * A071762  Leftmost 1 is converted to a 2, which then propagates one step at
 *          a time until it is rightmost; then it changes to a pair of 1's and
 *          the process repeats.
 * https://oeis.org/A071762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 65, u64 n <= 210, u128 n <= 819,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A071762: enumerating numbers with digits 1 and 2 where at most one 2
   appears, in the following order:
   For length L = 1,2,3,...
   • first output the all‑ones number of length L
   • then output the L numbers obtained by placing a single 2 at each
     position from left to right.
   Thus the sequence begins 0,1,2,11,21,12,111,211,121,112,...
   This implementation runs in O(√n) time for the block length L and
   uses only integer arithmetic. */

seq_t a(idx_t n)
{
    /* explicit value for the offset 0 term */
    if (n == 0)
        return 0;

    /* zero‑based offset among positive terms: a(1) has offset 0 */
    idx_t offset = n - (idx_t)1;

    /* find the block length L such that L(L+3)/2 > offset */
    idx_t L = 1;
    while ((L * (L + 3U)) / 2U <= offset)
        ++L;

    /* cumulative count of terms up to the block preceding L */
    idx_t before = ((L - 1U) * (L + 2U)) / 2U;

    /* position inside the current block (0 … L) */
    idx_t pos_in_block = offset - before;

    /* build the all‑ones number of length L */
    seq_t base = 0;
    for (idx_t i = 0; i < L; ++i)
        base = base * (seq_t)10 + (seq_t)1;

    if (pos_in_block == 0U) {
        /* all‑ones term */
        return base;
    } else {
        /* place a single 2 at the position pos_in_block-1 (left‑to‑right) */
        /* exponent for the 10‑power: L - pos_in_block */
        idx_t shift = L - pos_in_block;
        seq_t delta = 1;
        for (idx_t i = 0; i < shift; ++i)
            delta *= (seq_t)10;
        return base + delta;
    }
}
