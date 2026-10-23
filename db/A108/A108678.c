/*
 * A108678  a(n) = (n+1)^2*(n+2)*(2*n+3)/6.
 * https://oeis.org/A108678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^2*(n+2)*(2*n+3)/6
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 335, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic to avoid overflow of intermediate products. */
    unsigned __int128 nn = n;            /* promote index to 128 bits   */
    unsigned __int128 a  = nn + 1;       /* (n+1)                       */
    unsigned __int128 b  = nn + 2;       /* (n+2)                       */
    unsigned __int128 c  = 2*nn + 3;      /* (2n+3)                      */
    unsigned __int128 prod = a * a * b * c;
    prod /= 6;                           /* divide by 6 exactly          */
    return (seq_t)prod;                  /* cast back to the target type */
}
