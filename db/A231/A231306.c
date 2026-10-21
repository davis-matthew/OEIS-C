/*
 * A231306  Recurrence a(n) = a(n-2) + n^M for M=7, starting with a(0)=0,
 *          a(1)=1.
 * https://oeis.org/A231306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=a(n-2)+n^7 for n≥2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 21, u64 n <= 361, u128 n <= 9999,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;   /* a(0) */
    seq_t prev1 = 1;   /* a(1) */
    seq_t result = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* compute i^7 using an unsigned 128‑bit intermediate */
        __uint128_t p  = ( __uint128_t )i;   /* i      */
        __uint128_t p2 = p * p;              /* i²     */
        __uint128_t p4 = p2 * p2;            /* i⁴     */
        __uint128_t p7 = p4 * p2 * p;        /* i⁴·i²·i = i⁷ */

        result = prev2 + (seq_t)p7;          /* a(i)=a(i-2)+i^7 */
        prev2 = prev1;                       /* shift window */
        prev1 = result;
    }

    return result;
}
