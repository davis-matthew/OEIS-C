/*
 * A119972  a(n) = n * A034947(n).
 * https://oeis.org/A119972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(n) = n * Kronecker(-1, n) * = n * (odd_part(n) mod 4 == 1 ? 1
 *            : -1) * where odd_part(n) is n with all factors of 2 removed
 * Range:     i8 n <= 127, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    /* Remove all factors of 2 */
    while ((m & 1) == 0)
        m >>= 1;
    /* Determine the sign: 1 if odd part ≡ 1 (mod 4), else -1 */
    seq_t sign = (m & 3) == 1 ? (seq_t)1 : (seq_t)-1;
    return (seq_t)n * sign;
}
