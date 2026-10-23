/*
 * A143432  Ultimately period 4 sequence [ 2, 2, 0, 0, ...] with a(0) = a(1) =
 *          1.
 * https://oeis.org/A143432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { 1, n = 0 or 1; 2, n % 4 < 2 and n >= 2; 0, otherwise }
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows the definition: a(4*n+2) = a(4*n+3) = 0, a(4*n) = a(4*n+1) = 2 for n >= 1,
   with a(0) = a(1) = 1. */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;

    /* modulo 4 is cheap because 4 is a power of two */
    idx_t mod4 = n & 3;
    return (mod4 < 2) ? (seq_t)2 : (seq_t)0;
}
