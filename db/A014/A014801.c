/*
 * A014801  Squares of odd hexagonal pyramidal numbers.
 * https://oeis.org/A014801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((k*(k+1)*(4*k-1)/6)^2), where k = 4*⌊n/2⌋ + (n%2?2:1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 929, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the square of the odd hexagonal pyramidal number p(k). */

seq_t a(idx_t n)
{
    /* k = 4*floor(n/2) + (n%2?2:1) */
    idx_t half = n >> 1;                     /* floor(n/2) */
    seq_t k = 4 * (seq_t)half + (n & 1 ? 2 : 1);

    /* p = k*(k+1)*(4*k-1)/6 */
    seq_t t = k * (k + 1);                   /* k*(k+1) */
    t = t * (4 * k - 1);                     /* * (4*k-1) */
    seq_t p = t / 6;                         /* divide by 6 */

    /* return p*p */
    return p * p;
}
