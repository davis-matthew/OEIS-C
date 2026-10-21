/*
 * A055996  a(n) = 81*10^(n-2), a(0)=1, a(1)=8.
 * https://oeis.org/A055996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 81*10^(n-2) for n ≥ 2, with a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t result = (seq_t)81;          /* start with 81 = 81*10^0   */
    for (idx_t i = 2; i < n; ++i) {
        result *= (seq_t)10;           /* multiply by 10 to build 10^(n‑2) */
    }
    return result;
}
