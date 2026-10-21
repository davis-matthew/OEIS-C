/*
 * A099276  Unsigned member r=-18 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A099276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=18; for n>=3
 *            a(n)=19*a(n-1)+19*a(n-2)-a(n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 18;

    seq_t a0 = 0;   /* a(n-3) */
    seq_t a1 = 1;   /* a(n-2) */
    seq_t a2 = 18;  /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = 19 * a2 + 19 * a1; /* 19*a(n-1)+19*a(n-2) */
        an -= a0;                /* -a(n-3) */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
