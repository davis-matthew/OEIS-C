/*
 * A083328  a(n) = (6^n - 5^n + 4^n - 3^n)/2.
 * https://oeis.org/A083328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6^n - 5^n + 4^n - 3^n) / 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t s = 1; /* 6^0 */
    seq_t t = 1; /* 5^0 */
    seq_t u = 1; /* 4^0 */
    seq_t v = 1; /* 3^0 */

    for (idx_t i = 1; i <= n; ++i) {
        s = s * 6;
        t = t * 5;
        u = u * 4;
        v = v * 3;
    }

    seq_t val = s;
    val -= t;
    val += u;
    val -= v;
    val /= 2; /* division by two, result is always integer */

    return val;
}
