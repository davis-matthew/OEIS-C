/*
 * A216128  6^n mod 10000.
 * https://oeis.org/A216128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 10000
 * Range:     u8 n <= 1, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 10000;          /* modulus for the sequence       */
    seq_t result = 1;                 /* 6^0 modulo 10000, used as start */
    seq_t base   = 6 % MOD;           /* 6 modulo 10000                  */

    idx_t exp = n;                    /* exponent (n)                    */
    while (exp)                       /* binary exponentiation          */
    {
        if (exp & 1ULL)
        {
            unsigned long long mul_mod =
                (unsigned long long)result * (unsigned long long)base;
            result = (seq_t)(mul_mod % (unsigned long long)MOD);
        }

        unsigned long long mul_base =
            (unsigned long long)base * (unsigned long long)base;
        base = (seq_t)(mul_base % (unsigned long long)MOD);

        exp >>= 1;
    }

    return result;                    /* 6^n mod 10000                   */
}
