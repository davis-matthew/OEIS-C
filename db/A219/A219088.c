/*
 * A219088  a(n) = floor((n + 1/2)^5).
 * https://oeis.org/A219088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1/2)^5) = floor((2n+1)^5/32)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute x = 2n + 1 */
    seq_t x = ((seq_t)n << 1) + 1;

    /* Use a wider intermediate type to avoid overflow during the
       5th power computation.  (unsigned __int128 is available on
       common GCC/Clang targets.) */
    unsigned __int128 p = (unsigned __int128)x;  /* x^1   */
    p *= p;                                     /* x^2   */
    p *= p;                                     /* x^4   */
    p *= (unsigned __int128)x;                  /* x^5   */

    /* Divide by 32 by shifting right 5 bits. */
    p >>= 5; /* floor(x^5 / 32) */

    return (seq_t)p;
}
