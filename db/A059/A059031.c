/*
 * A059031  Fifth main diagonal of A059026: a(n) = B(n+4,n) = lcm(n+4,n)/(n+4)
 *          + lcm(n+4,n)/n - 1 for all n >= 1.
 * https://oeis.org/A059031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + 3 if n is odd (a(2k+1) = 4*k + 5) a(n) = n + 1 if n
 *            ≡ 2 (mod 4) (a(4k+2) = 4*k + 3) a(n) = n / 2 if n ≡ 0 (mod 4)
 *            (a(4k+4) = 4*k + 2) Defined for all n >= 1
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1) {                     /* odd n */
        return (seq_t)n * 2 + 3;
    } else if ((n & 3) == 2) {       /* n ≡ 2 (mod 4) */
        return (seq_t)n + 1;
    } else {                         /* n ≡ 0 (mod 4) */
        return (seq_t)n / 2;
    }
}
