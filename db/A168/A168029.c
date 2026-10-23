/*
 * A168029  a(n) = n*(n^6 + 1)/2.
 * https://oeis.org/A168029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^6 + 1)/2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 624,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow of the n^6 term
       (for uint64_t n this comfortably fits).  The division by 2 is
       always exact since n*(n^6+1) is even. */
    unsigned __int128 nn  = (unsigned __int128)n;
    unsigned __int128 n3  = nn * nn * nn;          /* n^3   */
    unsigned __int128 n6  = n3 * n3;               /* n^6   */
    unsigned __int128 res = nn * (n6 + 1) / 2;      /* n*(n^6+1)/2 */
    return (seq_t)res;
}
