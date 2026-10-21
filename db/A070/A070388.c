/*
 * A070388  a(n) = 5^n mod 42.
 * https://oeis.org/A070388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 42
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t mod   = (seq_t)42;
    seq_t   base = (seq_t)5;
    seq_t   result = (seq_t)1;
    idx_t   exp = n;

    while (exp) {
        if (exp & (idx_t)1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}
