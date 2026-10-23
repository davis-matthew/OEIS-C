/*
 * A105452  a(n) = numerator of (7*n - 1)/3.
 * https://oeis.org/A105452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of (7*n - 1)/3
 * Range:     u8 n <= 37, u16 n <= 9362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* offset starts at 1, but 0 is unused */

    switch (n % 3) {
    case 1:          /* n ≡ 1 (mod 3) → a(3k+1) = 7k + 2 */
        return 7 * ((seq_t)(n / 3)) + 2;
    case 2:          /* n ≡ 2 (mod 3) → a(3k+2) = 21k + 13 */
        return 21 * ((seq_t)(n / 3)) + 13;
    default:         /* n ≡ 0 (mod 3) → a(3k)   = 21k - 1 */
        return 21 * ((seq_t)(n / 3)) - 1;
    }
}
