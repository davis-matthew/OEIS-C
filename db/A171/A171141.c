/*
 * A171141  Numbers that are congruent to {6,33} mod 41.
 * https://oeis.org/A171141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (82*n + 13*(-1)^n - 45)/4
 * Range:     u8 n <= 3, u16 n <= 898, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n == 0)
        return 0; /* undefined below offset */

    seq_t nn = (seq_t)n;
    seq_t numer;

    /* (-1)^n is 1 for even n, -1 for odd n.
       Avoid negative intermediate by separating cases. */
    if ((n & 1) == 0) {          /* n even: 13 * (+1) */
        numer = 82 * nn - 32;    /* 82*n + 13 - 45 = 82*n - 32 */
    } else {                     /* n odd : 13 * (-1) */
        numer = 82 * nn - 58;    /* 82*n - 13 - 45 = 82*n - 58 */
    }

    return numer / 4;
}
