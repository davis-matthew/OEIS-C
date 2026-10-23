/*
 * A191901  Number of compositions of odd natural numbers into 6 parts <= n.
 * https://oeis.org/A191901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)^6 - (1+(-1)^n)/2)/2
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 44, u64 n <= 1823,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for the intermediate (n+1)^6 so
       that the result is exact for all n up to at least 10 000.   */
    unsigned __int128 b = (unsigned __int128)((seq_t)n + 1);
    unsigned __int128 p = b * b * b * b * b * b;      /* (n+1)^6   */
    unsigned __int128 sub = ((n & 1) == 0) ? 1 : 0;   /* (1+(-1)^n)/2 */
    p -= sub;                                          /* (n+1)^6 - sub */
    p >>= 1;                                           /* divide by 2   */
    return (seq_t)p;
}
