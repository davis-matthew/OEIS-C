/*
 * A003048  a(n+1) = n*a(n) - (-1)^n.
 * https://oeis.org/A003048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = n*a(n) - (-1)^n, with a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; i++) {
        result = (seq_t)(i - 1) * result;
        if (i % 2 == 0) {
            result += 1;
        } else {
            result -= 1;
        }
    }
    return result;
}
