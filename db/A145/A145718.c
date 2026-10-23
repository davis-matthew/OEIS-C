/*
 * A145718  Numbers x such that there exists n in N with (x+127)^3-x^3=n^2.
 * https://oeis.org/A145718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=762, a(2)=1676527, recurrence: a(n) = 2030*a(n−1) – a(n−2)
 *            + 128778 (n≥3) Cache is not used – the recurrence is computed
 *            directly
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out‑of‑range value (irrelevant) */

    if (n == 1) return (seq_t)762;
    if (n == 2) return (seq_t)1676527;

    seq_t a_nm2 = (seq_t)762;          /* a(n‑2) for n=3 starts with a(1) */
    seq_t a_nm1 = (seq_t)1676527;      /* a(n‑1) for n=3 starts with a(2) */
    seq_t a_n;                        /* result for current n */

    for (idx_t k = 3; k <= n; ++k)
    {
        a_n = (seq_t)2030 * a_nm1 - a_nm2 + (seq_t)128778;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
