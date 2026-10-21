/*
 * A231673  a(n) = Sum_{i=0..n} digsum_6(i)^2, where digsum_6(i) = A053827(i).
 * https://oeis.org/A231673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_6(i)^2, where digsum_6(i) =
 *            A053827(i)
 * Range:     u8 n <= 16, u16 n <= 783, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    const seq_t base = (seq_t)6;          /* base 6 */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t num = (seq_t)i;             /* number whose digits we sum */
        seq_t s   = 0;                   /* digsum_6(i) */
        while (num > 0) {
            s += num % base;             /* add least‑significant digit */
            num /= base;                 /* drop that digit */
        }
        result += s * s;                 /* accumulate square of the sum */
    }
    return result;
}
