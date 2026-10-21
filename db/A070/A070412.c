/*
 * A070412  a(n) = 7^n mod 27.
 * https://oeis.org/A070412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 27
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast binary modular exponentiation for 7^n modulo 27. */
    seq_t base = (seq_t)7;
    const seq_t mod  = (seq_t)27;
    seq_t result = (seq_t)1;

    while (n) {
        if (n & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        n >>= 1;
    }
    return result;
}
