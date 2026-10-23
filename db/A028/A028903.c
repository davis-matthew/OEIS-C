/*
 * A028903  Map n = Sum c_i 10^i to a(n) = Sum c_i 8^i.
 * https://oeis.org/A028903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum c_i * 8^i, where n = Sum c_i * 10^i (decimal digits
 *            c_i)
 * Range:     u8 n <= 377, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)0;
    seq_t mul = (seq_t)1;
    while (n != (idx_t)0) {
        idx_t digit = n % (idx_t)10;              /* least significant decimal digit */
        res += (seq_t)digit * mul;                /* add contribution of this digit */
        mul *= (seq_t)8;                          /* next power of 8 */
        n   /= (idx_t)10;                         /* move to next digit */
    }
    return res;
}
