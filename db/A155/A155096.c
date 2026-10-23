/*
 * A155096  Numbers k such that k^2 == -1 (mod 29).
 * https://oeis.org/A155096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*(-1)^(n+1) + 29*floor(n/2) (odd n: a(n) = 12 +
 *            29*((n-1)/2), even n: a(n) = 29*n/2 - 12)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined before the offset; harmless */
        return 0;

    idx_t k = n / 2;                      /* floor(n/2) */
    idx_t base = k * 29;                  /* 29*floor(n/2) */
    if (n & 1ULL)                         /* odd n → add 12 */
        return (seq_t)(base + 12);
    else                                  /* even n → subtract 12 */
        return (seq_t)(base - 12);
}
