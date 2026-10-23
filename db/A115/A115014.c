/*
 * A115014  a(n) = Sum_{i=1..n, gcd(6,i)=1} i.
 * https://oeis.org/A115014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n, gcd(6,i)=1} i
 * Range:     u8 n <= 40, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = S(n) - sum_{i<=n,i≡0 (mod 2)}i - sum_{i<=n,i≡0 (mod 3)}i
 *            + sum_{i<=n,i≡0 (mod 6)}i
 * Where S(n)=n(n+1)/2.  Uses inclusion–exclusion on the divisors 2 and 3. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, offset is 1 */
    seq_t N = (seq_t)n;
    seq_t total = N * (N + 1) / 2; /* sum of 1..n */

    /* sum of multiples of 2, 3, and 6 */
    idx_t m2 = n / 2;
    seq_t M2 = (seq_t)m2;
    seq_t sum2 = (seq_t)2 * (M2 * (M2 + 1) / 2);

    idx_t m3 = n / 3;
    seq_t M3 = (seq_t)m3;
    seq_t sum3 = (seq_t)3 * (M3 * (M3 + 1) / 2);

    idx_t m6 = n / 6;
    seq_t M6 = (seq_t)m6;
    seq_t sum6 = (seq_t)6 * (M6 * (M6 + 1) / 2);

    return total - sum2 - sum3 + sum6;
}
