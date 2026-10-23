/*
 * A047572  Numbers that are congruent to {1, 2, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6)+{1,2,4,5,6,7}[ (n-1)%6 ] (Offset 1)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */
    idx_t m = n - 1;               /* m = n-1, guaranteed non‑negative */
    seq_t q = (seq_t)(m / 6);       /* floor((n-1)/6) */
    seq_t r = (seq_t)(m % 6);       /* (n-1) mod 6, 0 <= r <= 5 */
    static const seq_t offs[6] = {1, 2, 4, 5, 6, 7};
    return 8 * q + offs[(unsigned)r];
}
