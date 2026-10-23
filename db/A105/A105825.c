/*
 * A105825  a(n) = sigma(n) (mod 5).
 * https://oeis.org/A105825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(n) mod 5
 * Range:     u8 n <= 1621, u16 n <= 9826, u32 n <= 9826, u64 n <= 9826,
 *            u128 n <= 9826, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, so n>=1 in practice */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t other = n / d;
            if (other != d) sum += (seq_t)other;
        }
    }
    return sum % (seq_t)5;
}
