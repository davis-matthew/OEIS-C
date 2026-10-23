/*
 * A036157  a(n) = 2^n mod 197.
 * https://oeis.org/A036157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 197
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast modular exponentiation using 2^196 ≡ 1 (mod 197) */
    idx_t e = n % 196;           /* reduce exponent modulo 196 */
    seq_t res  = (seq_t)1;
    seq_t base = (seq_t)2;

    while (e)
    {
        if (e & 1U)
            res = (res * base) % (seq_t)197;
        base = (base * base) % (seq_t)197;
        e >>= 1;
    }
    return res;
}
