/*
 * A070616  a(n) = n^5 mod 33.
 * https://oeis.org/A070616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 33
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since 33 < 2^8, we can safely compute in an unsigned
       32‑bit type and cast the result to seq_t. */
    const uint32_t MOD = 33;
    uint32_t m   = (uint32_t)(n % MOD);   /* reduced base (0..32)   */
    uint32_t res = 1;                     /* multiplicative identity */
    for (int i = 0; i < 5; ++i)           /* 5 multiplications for n^5 */
        res = (res * m) % MOD;
    return (seq_t)res;                    /* return the residue in seq_t  */
}
