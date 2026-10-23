/*
 * A126547  Number of base 26 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of length‑n sequences over digits 1..26 such that
 *            every pair of adjacent digits differs by at most 5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    enum { ALPH = 26 };            /* alphabet size */

    seq_t curr[ALPH];
    seq_t next[ALPH];

    /* base case: sequences of length 1 */
    for (idx_t d = 0; d < ALPH; ++d)
        curr[d] = (seq_t)1;

    /* DP for lengths 2 .. n */
    for (idx_t step = 1; step < n; ++step) {
        for (idx_t d = 0; d < ALPH; ++d) {
            seq_t sum = (seq_t)0;
            idx_t l = (d > 5 ? d - 5 : 0);
            idx_t r = (d + 5 < ALPH ? d + 5 : ALPH - 1);
            for (idx_t k = l; k <= r; ++k)
                sum += curr[k];
            next[d] = sum;
        }
        /* copy next to curr for next iteration */
        for (idx_t d = 0; d < ALPH; ++d)
            curr[d] = next[d];
    }

    /* total number of valid sequences of length n */
    seq_t total = (seq_t)0;
    for (idx_t d = 0; d < ALPH; ++d)
        total += curr[d];

    return total;
}
