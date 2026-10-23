/*
 * A105826  a(n) = sigma(n) (mod 7).
 * https://oeis.org/A105826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) mod 7 (the sum of all positive divisors of n,
 *            taken modulo 7)
 * Range:     u8 n <= 8822, u16 n <= 8822, u32 n <= 8822, u64 n <= 8822,
 *            u128 n <= 8822, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset starts at 1, but guard against 0 */
    unsigned __int128 sum = 0;        /* use 128‑bit to avoid overflow for large n */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            sum += i;                 /* divisor i */
            idx_t j = n / i;          /* complementary divisor */
            if (j != i)               /* avoid double‑counting square root */
                sum += j;
        }
    }
    return (seq_t)(sum % 7u);          /* result fits in a single small number */
}
