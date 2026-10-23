/*
 * A101602  Expansion of g.f. c(3x)^4, where c(x) is the g.f. of A000108.
 * https://oeis.org/A101602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((8n+12)/(3n+12))*((3n+3)/(n+3))*3^n*C(n+1) Recurrence:
 *            (n+4)*a(n) - 6*(3n+7)*a(n-1) + 36*(2n+1)*a(n-2) = 0 with a(0)=1,
 *            a(1)=12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)12;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)12;  /* a(1) */

    for (idx_t i = 2u; i <= n; ++i)
    {
#if defined(__SIZEOF_INT128__)
        __int128 prod1 = (__int128)a1 * (6 * (3 * i + 7));
        seq_t term1 = (seq_t)(prod1 / (i + 4u));

        __int128 prod2 = (__int128)a0 * (36 * (2 * i + 1));
        seq_t term2 = (seq_t)(prod2 / (i + 4u));
#else
        seq_t term1 = a1 * (seq_t)(6 * (3 * i + 7));
        term1 /= (i + 4u);

        seq_t term2 = a0 * (seq_t)(36 * (2 * i + 1));
        term2 /= (i + 4u);
#endif
        seq_t an = term1 - term2;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
