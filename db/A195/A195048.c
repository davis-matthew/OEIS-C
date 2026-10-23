/*
 * A195048  Concentric 19-gonal numbers.
 * https://oeis.org/A195048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (38*n*n + 15*((n%2==0)?0:-2))/8
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type to allow large values. */
    seq_t N = (seq_t)n;

    /* Compute 38*n*n.  The intermediary value can be large but stays within
       the capacity of seq_t (which may be 128‑bit or arbitrary precision).   */
    seq_t res = (seq_t)38 * N * N;

    /* For odd n we need to subtract 30, which is 15*(-2).  Subtracting only
       when n is odd prevents underflow on unsigned types.                */
    if (n & 1)
        res -= (seq_t)30;

    /* Divide by 8; the result is always an integer. */
    return res / 8;
}
