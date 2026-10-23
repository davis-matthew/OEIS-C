/*
 * A019429  Continued fraction for tan(1/6).
 * https://oeis.org/A019429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 9897, u32 n <= 9897, u64 n <= 9897,
 *            u128 n <= 9897, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A019429: Continued fraction for tan(1/6)
   a(0) = 0
   a(1) = 5
   for n >= 2:
       if n is even: a(n) = 1
       if n is odd : a(n) = 12 * ((n-1)/2) + 4
*/
seq_t a(idx_t n)
{
    if (n == 0u) {
        return 0;
    }
    if (n == 1u) {
        return 5;
    }
    if ((n & 1u) == 0u) {          /* even n (>=2) */
        return 1;
    }
    /* odd n (>=3): a(n) = 12 * ((n-1)/2) + 4 */
    idx_t k = (n - 1u) >> 1u;       /* k = (n-1)/2 */
    return ((seq_t)12 * (seq_t)k) + 4;
}
