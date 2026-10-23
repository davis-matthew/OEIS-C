/*
 * A008612  Molien series of 2-dimensional representation of SL(2,3).
 * https://oeis.org/A008612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/3) - floor(n/2) + (n even ? 1 : 0)
 * Range:     u8 n <= 334, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (equivalent to the OEIS recurrence a(n)=a(n-2)+a(n-3)-a(n-5) with
 *          initial values a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1) */

seq_t a(idx_t n)
{
    /* cast the unsigned 64‑bit index to the sequence type */
    seq_t N = (seq_t)n;

    /* compute the main part of the formula */
    seq_t val = 2 * (N / 3) - (N / 2);

    /* add the parity adjustment */
    if ((n & 1) == 0)
        val += 1;

    return val;
}
