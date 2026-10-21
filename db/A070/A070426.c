/*
 * A070426  a(n) = 7^n mod 44.
 * https://oeis.org/A070426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 44
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t mod  = (seq_t)44;
    seq_t result = 1;
    seq_t base   = (seq_t)7 % mod;
    idx_t e = n;

    /* Binary exponentiation – O(log n) */
    while (e > 0) {
        if (e & 1)
            result = (result * base) % mod;

        base = (base * base) % mod;
        e >>= 1;
    }
    return result;
}
