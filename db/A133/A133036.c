/*
 * A133036  Divisors of 1816.
 * https://oeis.org/A133036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   a(n) is the n-th divisor of 1816 (2^3 · 227) in ascending order
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 1;
    for (int e227 = 0; e227 <= 1; ++e227) {
        seq_t big_mult = e227 ? (seq_t)227 : (seq_t)1;
        seq_t pow2 = 1;
        for (int e2 = 0; e2 <= 3; ++e2) {
            seq_t divisor = pow2 * big_mult;
            if (count == n) return divisor;
            ++count;
            pow2 <<= 1;              /* multiply by 2 */
        }
    }
    return (seq_t)0;                  /* no divisor for n > 8 */
}
