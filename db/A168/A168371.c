/*
 * A168371  a(n) = n^5*(n^3 + 1)/2.
 * https://oeis.org/A168371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 * (n^3 + 1) / 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 279,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute in 128‑bit to avoid overflow on 64‑bit builds */
    unsigned __int128 x   = (unsigned __int128)n;   /* n          */
    unsigned __int128 x2  = x * x;                  /* n²         */
    unsigned __int128 x3  = x2 * x;                 /* n³         */
    unsigned __int128 x5  = x3 * x2;                /* n⁵         */
    unsigned __int128 prod = x5 * (x3 + 1);         /* n⁵·(n³+1)  */
    return (seq_t)(prod / 2);                       /* divide by 2 */
}
