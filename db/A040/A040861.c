/*
 * A040861  Continued fraction for sqrt(891).
 * https://oeis.org/A040861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) defined multiplicatively. a(0) = 29, a(1) = 1. For n > 0
 *            let 2^e be the exact power of 2 dividing n. Then: e = 0 -> a(n)
 *            = 1 e = 1 -> a(n) = 5 e = 2 -> a(n) = 1 e ≥ 3 -> a(n) = 58 For
 *            any odd prime p, a(p^k) = 1, and the function is multiplicative
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)29;
    idx_t t = n;
    int e = 0;
    while ((t & 1) == 0) {
        e++;
        t >>= 1;
    }
    if (e == 0) return (seq_t)1;
    if (e == 1) return (seq_t)5;
    if (e == 2) return (seq_t)1;
    return (seq_t)58;
}
