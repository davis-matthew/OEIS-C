/*
 * A204689  a(n) = n^n (mod 4).
 * https://oeis.org/A204689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n (mod 4)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* Definition: 0^0 is taken as 1 */
        return (seq_t)1;

    /* Even n gives 0 modulo 4 (except n = 0 handled above) */
    if ((n & 1ULL) == 0)
        return (seq_t)0;

    /* n is odd; determine n modulo 4 */
    idx_t r = n & 3ULL;       /* r == n % 4 */

    if (r == 1)               /* 1^anything ≡ 1 (mod 4) */
        return (seq_t)1;

    /* r == 3: powers of 3 modulo 4 alternate 3,1,3,1,... */
    return (seq_t)3;
}
