/*
 * A234267  Expansion of (1-x)/((1-2*x)*(1-5*x+6*x^2-x^3)).
 * https://oeis.org/A234267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1)-16*a(n-2)+13*a(n-3)-2*a(n-4) for n>3 Initial
 *            values: a(0)=1, a(1)=6, a(2)=26, a(3)=99
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    static const seq_t init[4] = {1, 6, 26, 99};
    if (n < 4) return init[n];

    /* Work only with the last four computed values */
    seq_t v0 = init[0];   /* a(k-4) */
    seq_t v1 = init[1];   /* a(k-3) */
    seq_t v2 = init[2];   /* a(k-2) */
    seq_t v3 = init[3];   /* a(k-1) */

    /* Compute the sequence up to the requested n */
    for (idx_t k = 4; k <= n; ++k)
    {
        seq_t next = 7 * v3 - 16 * v2 + 13 * v1 - 2 * v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }
    return v3;
}
