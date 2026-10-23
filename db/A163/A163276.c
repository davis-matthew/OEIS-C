/*
 * A163276  a(n) = n^6*(n+1)^2/2.
 * https://oeis.org/A163276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^6*(n+1)^2/2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 17, u64 n <= 278, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n^6 * (n+1)^2 / 2 in arbitrary precision (using __int128 for safety) */
    unsigned __int128 x = n;                 /* cast index to 128‑bit type */
    unsigned __int128 prod = x * x * x * x * x * x;   /* n^6 */
    unsigned __int128 np1 = x + 1;
    prod = prod * np1 * np1 / 2;              /* (n+1)^2 / 2 */
    return (seq_t)prod;                       /* cast back to seq_t */
}
