/*
 * A047599  Numbers that are congruent to {0, 3, 4, 5} mod 8.
 * https://oeis.org/A047599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - 2 - cos(n*π/2) (OEIS A047599)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The cosine term cycles over {1,0,-1,0} for n mod 4 = {0,1,2,3}. */
    seq_t two_n = (seq_t)n * 2;
    seq_t res = two_n - 2;
    idx_t r = n & 3;                     /* n % 4 */
    if (r == 0)                          /* cos =  1  -> subtract 1    */
        res = res - 1;
    else if (r == 2)                     /* cos = -1  -> add 1         */
        res = res + 1;
    /* r == 1 or 3: cos = 0, no change */
    return res;
}
