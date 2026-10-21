/*
 * A066441  a(n) = 11^n mod n.
 * https://oeis.org/A066441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11^n mod n
 * Range:     u8 n <= 265, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Under the OEIS offset n ≥ 1, but guard against 0 for safety. */
    if (n == 0) return 0;

    seq_t mod = (seq_t)n;              /* modulus n as seq_t */
    seq_t base = (seq_t)11 % mod;      /* 11 mod n */
    seq_t result = (seq_t)1 % mod;     /* 1 mod n (initially 1) */

    idx_t e = n;
    while (e > 0) {
        if (e & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return result;
}
