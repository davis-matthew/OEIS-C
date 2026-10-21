/*
 * A070703  a(n) = n^7 mod 22.
 * https://oeis.org/A070703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 22
 * Range:     u8 n <= 275, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t mod = (seq_t)22;
    seq_t x = (seq_t)n % mod;   /* reduce n modulo 22 */
    seq_t res = (seq_t)1;
    unsigned int e = 7;          /* exponent 7 */

    while (e) {
        if (e & 1)
            res = (res * x) % mod;  /* multiply result by current power */
        x = (x * x) % mod;          /* square the base for next bit */
        e >>= 1;
    }
    return res;
}
