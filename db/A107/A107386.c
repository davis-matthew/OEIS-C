/*
 * A107386  a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4), n>6.
 * https://oeis.org/A107386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2/2 - 3*n/2 + 1 - (-1)^n a(0) = 0, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;

    seq_t nn = (seq_t)n;
    seq_t term1 = nn * nn / 2;            // n^2 / 2
    seq_t term2 = 3 * nn / 2;             // 3*n / 2
    seq_t res   = term1 - term2 + (seq_t)1;

    if ((n & 1) == 0) /* even n: subtract 1 */
        res -= (seq_t)1;
    else              /* odd n: add 1 (subtract -1) */
        res += (seq_t)1;

    return res;
}
