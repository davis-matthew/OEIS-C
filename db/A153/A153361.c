/*
 * A153361  Number of zig-zag paths from top to bottom of a rectangle of width
 *          12 with n rows.
 * https://oeis.org/A153361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let dp[i] be the number of words of the current length ending
   with symbol (i+1).  Initially dp[i]=1 for all 0≤i<12.  For each
   additional position update newdp[i] = (i>0?dp[i-1]:0)+(i<11?dp[i+1]:0).
   The sequence value a(n) is sum_i dp[i] after n-1 updates.  a(1)=12. */

seq_t a(idx_t n)
{
    /* a(1) is trivial */
    if (n <= 1) return (seq_t)12;

    /* dp for previous length and current length */
    seq_t prev[12], curr[12];

    /* base case: length 1, each symbol appears once */
    for (int i = 0; i < 12; ++i) {
        prev[i] = (seq_t)1;
    }

    /* iterate until we reach length n */
    for (idx_t step = 1; step < n; ++step) {   /* step counts current length */
        for (int i = 0; i < 12; ++i) {
            seq_t val = 0;
            if (i > 0)   val += prev[i - 1];    /* step to the left */
            if (i < 11)  val += prev[i + 1];    /* step to the right */
            curr[i] = val;
        }
        /* copy current to previous for next iteration */
        for (int i = 0; i < 12; ++i) {
            prev[i] = curr[i];
        }
    }

    /* sum over all ending positions */
    seq_t total = 0;
    for (int i = 0; i < 12; ++i) {
        total += prev[i];
    }
    return total;
}
