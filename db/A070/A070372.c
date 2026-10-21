/*
 * A070372  a(n) = 5^n mod 18.
 * https://oeis.org/A070372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 18
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast modular exponentiation: result = 5^n mod 18 */
    seq_t base = (seq_t)5;
    seq_t result = (seq_t)1;
    /* mod value fits in seq_t */
    const seq_t modulus = (seq_t)18;

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % modulus;
        base = (base * base) % modulus;
        exp >>= 1;
    }
    return result;
}
