/*
 * A008736  Molien series for 3-dimensional group [2+,n] = 2*(n/2).
 * https://oeis.org/A008736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n + 11 + 9*(-1)^n)^2 / 256) - [(n mod 16)=7] -
 *            [(n mod 16)=14]
 * Range:     u8 none, u16 n <= 643, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute t = 2*n + 11 + 9*(-1)^n.
       For even n, add 9; for odd n, subtract 9. */
    seq_t t = ((seq_t)n * 2) + 11;
    if (n & 1) {
        t -= 9;
    } else {
        t += 9;
    }

    /* Square and divide by 256 (floor). */
    seq_t sq = t * t;
    seq_t res = sq / 256;

    /* Subtract corrections for n mod 16 == 7 and n mod 16 == 14. */
    idx_t mod16 = n % 16;
    if (mod16 == 7)
        res--;
    if (mod16 == 14)
        res--;

    return res;
}
