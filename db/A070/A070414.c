/*
 * A070414  a(n) = 7^n mod 30.
 * https://oeis.org/A070414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 30
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use binary exponentiation with a small modulus.  All intermediate
       calculations are performed in a wider integer type to avoid
       overflow when seq_t is small (e.g., uint8_t). */
    const seq_t MOD = (seq_t)30;   /* 30 fits in any seq_t */
    seq_t result = (seq_t)1 % MOD; /* a(0) = 1 */
    seq_t base   = (seq_t)7 % MOD; /* 7 mod 30 = 7 */
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1U) {
            /* Promote to a wider type before multiplication */
            result = (seq_t)((unsigned long long)result *
                             (unsigned long long)base % (unsigned long long)MOD);
        }
        base = (seq_t)((unsigned long long)base *
                       (unsigned long long)base % (unsigned long long)MOD);
        exp >>= 1U;
    }
    return result;
}
