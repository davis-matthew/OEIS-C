/*
 * A143988  Numbers congruent to {5, 13} mod 18.
 * https://oeis.org/A143988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((-1)^(1+n) + 18*n - 9)/2
 * Range:     u8 n <= 14, u16 n <= 4403, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t   = (seq_t)n * 18u;     /* 18 * n  */
    seq_t num = t - 9u;            /* 18*n - 9 */

    /* Add or subtract 1 depending on the parity of n */
    if ((n & 1u) == 0u) {          /* n even → (-1)^(1+n) = -1 */
        num -= 1u;
    } else {                      /* n odd  → (-1)^(1+n) = +1 */
        num += 1u;
    }

    return num / 2u;               /* final division by 2 */
}
