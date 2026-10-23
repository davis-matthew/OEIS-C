/*
 * A021236  Decimal expansion of 1/232.
 * https://oeis.org/A021236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/232) mod 10 Equivalent: compute 10^(n+1)
 *            modulo 2320 and divide by 232
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t e = n + 1U;           /* exponent n+1 */
    unsigned long long m = 2320ULL;      /* modulus 2320 */
    unsigned long long b = 10ULL;        /* base 10 */
    unsigned long long r = 1ULL;         /* result accumulator */

    /* fast modular exponentiation */
    while (e) {
        if (e & 1ULL)
            r = (r * b) % m;
        b = (b * b) % m;
        e >>= 1ULL;
    }

    /* digit is floor(r / 232) */
    unsigned long long digit = r / 232ULL;
    return (seq_t)digit;
}
