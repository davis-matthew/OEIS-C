/*
 * A070585  a(n) = n^4 mod 42.
 * https://oeis.org/A070585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 42
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = (seq_t)(n % 42);      /* reduce modulo 42 first */
    seq_t r2 = r * r;               /* r² */
    seq_t r3 = r2 * r;              /* r³ */
    seq_t r4 = r3 * r;              /* r⁴ */
    return r4 % 42;                 /* final result modulo 42 */
}
