/*
 * A187334  Sum{floor(kn/5), k=1,2,3,4,5}; complement of A187335.
 * https://oeis.org/A187334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*floor(n/5) + S[n mod 5] where S = {0,1,4,7,10}
 * Range:     u8 n <= 85, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t S[5] = {0, 1, 4, 7, 10};
    seq_t q = ((seq_t)n) / 5;        /* floor(n/5) */
    idx_t r = n % 5;
    return (15 * q) + S[r];
}
