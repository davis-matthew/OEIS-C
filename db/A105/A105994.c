/*
 * A105994  Fibonacci sequence (mod 13).
 * https://oeis.org/A105994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 13
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)13;
    idx_t k = n % 28;          /* Pisano period for modulo 13 is 28 */
    if (k == 0) return (seq_t)0;
    if (k == 1) return (seq_t)1;
    seq_t a0 = 0, a1 = 1, a2;
    idx_t i;
    for (i = 2; i <= k; ++i) {
        a2 = a0 + a1;
        a2 %= MOD;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
