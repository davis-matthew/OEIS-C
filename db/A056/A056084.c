/*
 * A056084  Numbers k such that k^8 == 1 (mod 9^3).
 * https://oeis.org/A056084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
    OEIS A056084: Numbers k such that k^8 ≡ 1 (mod 9³).
    Formula: a(n) = 729 * floor(n/2) - (-1)^n
    (for n ≥ 1; a(1)=1, a(2)=728, ...)
*/
seq_t a(idx_t n)
{
    /* floor(n/2) fits in seq_t because n is unsigned */
    seq_t m = (seq_t)(n >> 1);    /* m = floor(n/2) */

    /* (-1)^n is +1 for odd n, -1 for even n. */
    if ((n & 1) == 0) {          /* even n >= 2 : a = 729*m - 1 */
        return 729 * m - 1;
    } else {                     /* odd n  : a = 729*m + 1 (covers n=1 too) */
        return 729 * m + 1;
    }
}
