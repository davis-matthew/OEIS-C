/*
 * A122501  Expansion of -x*(47*x^3+25*x^2+5*x+1)/(38*x^4+54*x^3+22*x^2-1).
 * https://oeis.org/A122501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(1)=1, a(2)=5, a(3)=47, a(4)=211, a(n) = 22*a(n-2) + 54*a(n-3)
 *            + 38*a( n-4) for n >= 5 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 1, safe guard */

    /* initial values a(1)..a(4) */
    static const seq_t init[4] = {1, 5, 47, 211};

    if (n <= 4) return init[n - 1];

    /* keep last four terms: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a1 = init[0];  /* a(n-4) */
    seq_t a2 = init[1];  /* a(n-3) */
    seq_t a3 = init[2];  /* a(n-2) */
    seq_t a4 = init[3];  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 22 * a3 + 54 * a2 + 38 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
