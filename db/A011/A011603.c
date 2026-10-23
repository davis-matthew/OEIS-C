/*
 * A011603  Legendre symbol (n,101).
 * https://oeis.org/A011603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   Legendre symbol (n,101) computed via Euler's criterion n^50 mod
 *            101
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 101; the result fits in any signed integer type */
    seq_t m = (seq_t)(n % 101);
    if (m == 0)            /* multiple of 101 gives value 0 */
        return (seq_t)0;

    /* Compute m^50 mod 101 using fast exponentiation */
    seq_t result = 1;
    seq_t base   = m;
    idx_t exp    = 50;          /* exponent (p-1)/2 for prime p = 101 */

    while (exp) {
        if (exp & 1)
            result = (result * base) % (seq_t)101;
        base = (base * base) % (seq_t)101;
        exp >>= 1;
    }

    /* In Legendre symbol terms, result is 1 for quadratic residues,
       100 ≡ –1 (mod 101) for non-residues */
    if (result == 1)
        return (seq_t)1;
    else
        return (seq_t)(-1);
}
