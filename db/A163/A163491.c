/*
 * A163491  A fractal sequence (if we delete the first occurrence of n we get
 *          the sequence itself).
 * https://oeis.org/A163491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)/3 if n % 3 == 1, otherwise a(n) = a(floor(2*n/3))
 * Range:     u8 n <= 765, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset starts at 1, but guard for 0 */
    while (1) {
        if (n % 3 == 1) {
            /* (n+2) and division performed on idx_t to avoid overflow;
               the result fits into seq_t for all tested n.           */
            return (seq_t)((n + 2) / 3);
        }
        /* compute floor(2*n/3) without overflow:
           let n = 3*m + r, 0 <= r < 3
           then 2*n/3 = 2*m + (2*r)/3  (integer division)          */
        idx_t m = n / 3;
        idx_t r = n % 3;
        n = m * 2 + (r * 2) / 3;
    }
}
