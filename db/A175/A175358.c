/*
 * A175358  Irregular array read by rows: Row n contains A043276(n) terms.
 *          a(n,m) = number of runs (of either 0 or 1) of length m in the
 *          binary representation of n.
 * https://oeis.org/A175358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabf
 * Formula:   a(n) = n-th term of the irregular array read by rows. Row k
 *            (k≥1) contains counts of runs of length m in the binary
 *            representation of k, for m = 1 … maxRun(k), where maxRun(k) is
 *            the longest run of equal bits in k. The sequence is flattened
 *            row by row and a(n) is the n-th element (1-indexed)
 * Range:     u8 n <= 7473, u16 n <= 7473, u32 n <= 7473, u64 n <= 7473,
 *            u128 n <= 7473, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* undefined for 0, but return 0 for safety */
        return (seq_t)0;

    idx_t     term_pos = 0;   /* total terms generated so far */
    idx_t     row     = 1;    /* current binary number */

    for (;; ++row) {
        /* compute run counts for 'row' */
        idx_t   x = row;
        seq_t   counts[65] = {0};   /* index by run length, 1..64 */
        uint64_t run_len   = 0;
        int     prev_bit   = -1;
        uint64_t max_len   = 0;

        /* iterate over bits of x (LSB to MSB) */
        while (x) {
            int bit = (int)(x & 1);
            if (bit == prev_bit) {
                run_len++;
            } else {
                if (prev_bit != -1) {
                    counts[run_len] += (seq_t)1;
                    if (run_len > max_len) max_len = run_len;
                }
                prev_bit = bit;
                run_len  = 1;
            }
            x >>= 1;
        }
        if (prev_bit != -1) {          /* last run */
            counts[run_len] += (seq_t)1;
            if (run_len > max_len) max_len = run_len;
        }

        /* output this row's terms */
        for (uint64_t m = 1; m <= max_len; ++m) {
            term_pos++;
            if (term_pos == n) {
                return counts[m];
            }
        }
    }
}
