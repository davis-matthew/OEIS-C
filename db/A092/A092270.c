/*
 * A092270  If n mod 2 == 0 then 3^n else 2^n.
 * https://oeis.org/A092270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n mod 2 == 0) ? 3^n : 2^n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base = (n & 1) ? (seq_t)2 : (seq_t)3;
    idx_t exp = n;
    while (exp) {
        if (exp & 1u)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}
