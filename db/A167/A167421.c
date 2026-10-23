/*
 * A167421  2^n mod 22.
 * https://oeis.org/A167421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 22 Initial terms: a(0)=1, a(1)=2, a(2)=4, a(3)=8,
 *            a(4)=16, a(5)=10, a(6)=20, a(7)=18, a(8)=14, a(9)=6, a(10)=12, …
 *            (periodic with period 10 after n=0)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Modulus */
    const seq_t MOD = (seq_t)22;

    /* Since 2^n mod 22 has period 10 for n >= 1, reduce n modulo 10.
       For n % 10 == 0 we need 2^10 mod 22 (=12). */
    idx_t k = n % (idx_t)10;
    if (k == 0) {
        k = 10;            /* treat as exponent 10 */
    }

    /* Fast modular exponentiation with small modulus.
       All intermediate numbers are kept below 22, so overflow is avoided. */
    seq_t result = 1;
    seq_t base   = 2;
    idx_t exp    = k;

    while (exp > 0) {
        if (exp & 1) {
            unsigned int prod = (unsigned int)result * (unsigned int)base;
            result = (seq_t)(prod % (unsigned int)MOD);
        }
        unsigned int prod2 = (unsigned int)base * (unsigned int)base;
        base = (seq_t)(prod2 % (unsigned int)MOD);
        exp >>= 1;
    }

    return result;
}
