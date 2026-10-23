/*
 * A011623  Legendre symbol (n,199).
 * https://oeis.org/A011623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,199) = n^99 (mod 199) mapped to
 *            {-1,0,1}
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n^99 mod 199 using binary exponentiation */
    const int MOD   = 199;
    int base        = (int)(n % MOD);
    int exp         = 99;
    int result      = 1;

    while (exp) {
        if (exp & 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }

    /* Map the modular result to the Legendre symbol value */
    if (result == 0)
        return (seq_t)0;
    if (result == 1)
        return (seq_t)1;
    /* The only other possibility is 198, which represents -1 */
    return (seq_t)(-1);
}
