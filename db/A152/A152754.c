/*
 * A152754  "Upper positive integers": n is in the sequence iff in the
 *          representation n=A000695(k)+2*A000695(l) satisfies inequality
 *          A000695(k) < A000695(l).
 * https://oeis.org/A152754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 8128, u32 n <= 8128, u64 n <= 8128,
 *            u128 n <= 8128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152754 – Upper positive integers
 *
 * For an integer N, write its binary expansion as
 *        N = Σ b_j · 2^j    (b_j ∈ {0,1}).
 * Split the bits into even and odd positions:
 *        E = Σ b_{2j} · 2^j      (even-position bits reinterpreted as a binary number)
 *        O = Σ b_{2j+1} · 2^j    (odd-position bits reinterpreted similarly)
 *
 * N is in the sequence iff E < O.
 *
 * The function returns the n-th term (a(1) = 2).
 */
seq_t a(idx_t n)
{
    idx_t m      = 2;          /* start searching from 2 */
    idx_t found  = 0;          /* number of qualifying terms found */

    while (1) {
        seq_t even = 0;
        seq_t odd  = 0;
        idx_t   bitpos = 0;
        idx_t   tmp    = m;

        /* decompose m into even/odd bit groups */
        while (tmp) {
            if (tmp & 1) {
                if ((bitpos & 1) == 0) {               /* even position */
                    even |= ((seq_t)1 << (bitpos >> 1));
                } else {                               /* odd position */
                    odd  |= ((seq_t)1 << ((bitpos - 1) >> 1));
                }
            }
            bitpos++;
            tmp >>= 1;
        }

        if (even < odd) {
            found++;
            if (found == n) {
                return (seq_t)m;
            }
        }
        m++;
    }
}
