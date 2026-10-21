/*
 * A067275  Number of Fibonacci numbers A000045(k), k <= 10^n, which end in 4.
 * https://oeis.org/A067275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ceil(10^n / 15) = (10^n + 5)/15 for n>=1, recurrence:
 *            a(0)=0, a(1)=1, a(n)=10*a(n-1)-3 for n>1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t res = 1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        res = res * 10 - 3; /* a(i) = 10*a(i-1)-3 */
    }
    return res;
}
