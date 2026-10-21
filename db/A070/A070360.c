/*
 * A070360  a(n) = 3^n mod 38.
 * https://oeis.org/A070360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 38
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Constants used in the recurrence */
    const seq_t MOD = 38;      /* Modulus */
    const seq_t BASE = 3;      /* Base of exponentiation */

    seq_t result = 1;          /* 3^0 mod 38 = 1 */
    seq_t cur = BASE;          /* Current power of 3 modulo 38 */
    idx_t e = n;               /* Exponent copied to a mutable variable */

    while (e) {
        /* If the current bit of the exponent is set, multiply into the result.
           Use a wider integer type (unsigned long long) for the intermediate
           multiplication so that no overflow occurs on small seq_t types. */
        if (e & 1)
            result = (seq_t)(((unsigned long long)result * cur) % MOD);

        /* Square the current term for the next bit.  Again use a wider type to
           avoid overflow before taking the modulo. */
        cur = (seq_t)(((unsigned long long)cur * cur) % MOD);

        e >>= 1; /* Move to the next bit of the exponent */
    }

    return result;
}
