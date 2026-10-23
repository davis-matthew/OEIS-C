/*
 * A036197  Log base 2 (n) mod 181.
 * https://oeis.org/A036197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the discrete logarithm base 2 of n modulo the prime 181.
 *            That is, a(n) = e such that 2^e ≡ n (mod 181). The sequence has
 *            offset 1: a(1) = 0 because 2^0 ≡ 1 (mod 181). For n divisible by
 *            181 the value is defined as 0
 * Range:     u8 n <= 180, u16 n <= 180, u32 n <= 180, u64 n <= 180,
 *            u128 n <= 180, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t MOD = 181;
    idx_t m = n % MOD;            /* reduce n modulo 181 */
    if (m == 0) return 0;         /* n ≡ 0 (mod 181) has no discrete log */

    idx_t val = 1;                /* 2^0 mod 181 */
    for (idx_t e = 0; e < MOD - 1; ++e) {
        if (val == m)
            return (seq_t)e;      /* cast to the return type */
        val = (val * 2) % MOD;     /* next power of 2 modulo 181 */
    }
    return 0;  /* this point is never reached for n not ≡ 0 (mod 181) */
}
