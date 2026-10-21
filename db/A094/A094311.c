/*
 * A094311  a(n) = n*(1+n^2)*((2+n^2)^2-n^2)/16.
 * https://oeis.org/A094311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1+n^2)*((2+n^2)^2-n^2)/16
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 35, u64 n <= 840, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses 128‑bit temporaries to avoid overflow
   during intermediate calculations.  The division by 16 is
   performed by an arithmetic right shift. */
seq_t a(idx_t n)
{
    /* cast to 128‑bit to hold intermediate values safely */
    unsigned __int128 nn  = n;
    unsigned __int128 n2  = nn * nn;          /* n^2   */
    unsigned __int128 n4  = n2 * n2;          /* n^4   */
    unsigned __int128 term1 = nn * (n2 + 1);  /* n * (1 + n^2) */
    unsigned __int128 term2 = n4 + 3 * n2 + 4;/* (2+n^2)^2 - n^2 = n^4 + 3n^2 + 4 */
    unsigned __int128 prod = term1 * term2;   /* product */
    prod >>= 4;                               /* divide by 16 */
    return (seq_t)prod;
}
