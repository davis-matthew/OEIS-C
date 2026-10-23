/*
 * A155086  Numbers k such that k^2 == -1 (mod 13).
 * https://oeis.org/A155086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 + 13*((n-1)/2) for odd n, 8 + 13*((n-2)/2) for even n
 *            (OEIS A155086)
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not defined for n < 1; any value is acceptable */

    if (n & 1) {              /* odd indices */
        seq_t k = (seq_t)(((n - 1) / 2) * (idx_t)13);
        return (seq_t)5 + k;
    } else {                  /* even indices */
        seq_t k = (seq_t)(((n - 2) / 2) * (idx_t)13);
        return (seq_t)8 + k;
    }
}
