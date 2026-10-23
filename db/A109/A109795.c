/*
 * A109795  a(n)= n*(1+floor(n/10)).
 * https://oeis.org/A109795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(1+floor(n/10))
 * Range:     u8 n <= 49, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is zero‑based only for the compiler; OEIS offset is 1. */
    seq_t sn = (seq_t)n;          /* cast index to sequence type */
    seq_t t  = sn / (seq_t)10;    /* floor(n/10) */
    return sn * (t + (seq_t)1);
}
