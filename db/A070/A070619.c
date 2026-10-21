/*
 * A070619  a(n) = n^5 mod 36.
 * https://oeis.org/A070619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 36
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 36 first; this guarantees intermediate values stay small. */
    const seq_t MOD  = (seq_t)36;
    const seq_t base = (seq_t)(n % (idx_t)36ULL);

    seq_t res = 1;
    for (int i = 0; i < 5; ++i)      /* multiply five times to compute base^5 */
        res = (res * base) % MOD;

    return res;
}
