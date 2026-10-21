/*
 * A059291  a(n) = round((n-1)^2*(n+5)/144).
 * https://oeis.org/A059291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((n-1)^2*(n+5)/144)
 * Range:     u8 n <= 32, u16 n <= 210, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a signed 128‑bit integer to avoid overflow for any n that fits in idx_t */
    __int128 N   = ( __int128 )n;        /* original n                      */
    __int128 nm1 = N - 1;                /* (n-1)                            */
    __int128 num = nm1 * nm1 * (N + 5);   /* (n-1)^2 * (n+5)                  */
    num += 72;                           /* +0.5 * 144  for rounding          */
    num /= 144;                          /* floor( (num + 72) / 144 )        */
    return (seq_t)num;                   /* cast back to seq_t                */
}
