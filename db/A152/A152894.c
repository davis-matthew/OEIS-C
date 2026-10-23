/*
 * A152894  Periodic sequence [0,0,1,4,0] of period 5.
 * https://oeis.org/A152894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^3 - n) mod 5
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t MOD = 5;
    idx_t m   = n % MOD;        /* n mod 5 */
    idx_t cube = m * m * m;     /* m^3, small value */
    idx_t rem  = (cube - m) % MOD; /* (m^3 - m) mod 5 */
    return (seq_t)rem;
}
