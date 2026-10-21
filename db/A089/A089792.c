/*
 * A089792  a(n) = n - (exponent of highest power of 3 dividing n!).
 * https://oeis.org/A089792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - sum_{k>=1} floor(n/3^k)
 * Range:     u8 n <= 505, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Legendre's formula for p = 3:  v3(n!) = Σ_{k≥1} ⌊n/3^k⌋ */
    seq_t v = 0;
    if (n > 0) {
        idx_t power = 3;
        while (power <= n) {
            v += (seq_t)(n / power);
            if (power > n / 3) {
                /* next multiplication would overflow or exceed n */
                break;
            }
            power *= 3;
        }
    }
    return (seq_t)n - v;
}
