/*
 * A010025  Crystal ball sequence for squashed {D_5}^* lattice, perhaps the
 *          smallest example of a "non-superficial" lattice.
 * https://oeis.org/A010025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^5 + 15*n^4 + 30*n^3 + 45*n^2 + 46*n + 12)/12
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 91, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow for 64‑bit seq_t
       while also fitting in the arbitrary‑precision build. */
    unsigned __int128 t = 8;          /* start with coefficient of n^5 */
    t = t * n + 15;                   /* add coefficient of n^4 */
    t = t * n + 30;                   /* add coefficient of n^3 */
    t = t * n + 45;                   /* add coefficient of n^2 */
    t = t * n + 46;                   /* add coefficient of n   */
    t = t * n + 12;                   /* add constant term       */
    t = t / 12;                       /* final division by 12    */
    return (seq_t)t;                  /* cast back to seq_t      */
}
