/*
 * A070379  a(n) = 5^n mod 29.
 * https://oeis.org/A070379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 29
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast modular exponentiation using binary exponentiation */
    seq_t result = 1;          /* a(0) = 1 */
    seq_t base   = 5;          /* base 5 modulo 29 */
    seq_t mod    = 29;         /* modulus 29 */

    while (n > 0) {
        if (n & 1U) {
            /* result = (result * base) mod mod */
            result = (seq_t)((unsigned long long)result * base % mod);
        }
        /* base = (base * base) mod mod */
        base = (seq_t)((unsigned long long)base * base % mod);
        n >>= 1U;
    }
    return result;
}
