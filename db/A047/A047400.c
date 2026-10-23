/*
 * A047400  Numbers that are congruent to {1, 3, 6} mod 8.
 * https://oeis.org/A047400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/3) + {1,3,6}[ (n-1)%3 ]
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Indexing starts at 1; behaviour for n==0 is undefined per OEIS offset */
    seq_t n_seq = (seq_t)n;
    seq_t q = (n_seq - 1) / 3;           /* block number */
    seq_t r = (n_seq - 1) % 3;           /* position within the block (0,1,2) */
    seq_t offset = (r == 0) ? (seq_t)1
                 : (r == 1) ? (seq_t)3
                 : (seq_t)6;
    return q * 8 + offset;
}
