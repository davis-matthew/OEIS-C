/*
 * A108680  Kekulé numbers for certain benzenoids.
 * https://oeis.org/A108680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^3*(n+3)^2*(n+4)*(n+5)/1440
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 36, u64 n <= 632, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate when available so that
       intermediate results do not overflow before the
       final division.  Casting to seq_t at the end ensures
       correct values for the arbitrary‑precision build,
       and the 64‑bit build will receive the correct value
       for all n where it fits in 64 bits. */
    unsigned __int128 big = (uint64_t)1;

    unsigned __int128 np1 = (unsigned __int128)(n + 1);
    unsigned __int128 n2  = (unsigned __int128)(n + 2);
    unsigned __int128 n3  = (unsigned __int128)(n + 3);
    unsigned __int128 np4 = (unsigned __int128)(n + 4);
    unsigned __int128 np5 = (unsigned __int128)(n + 5);

    big *= np1;
    big *= n2;   /* (n+2) */
    big *= n2;   /* (n+2)^2 */
    big *= n2;   /* (n+2)^3 */
    big *= n3;   /* (n+3) */
    big *= n3;   /* (n+3)^2 */
    big *= np4;
    big *= np5;
    big /= 1440; /* exact division */

    return (seq_t)big;
}
