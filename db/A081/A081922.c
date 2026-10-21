/*
 * A081922  Expansion of exp(4x)/sqrt(1-x^2).
 * https://oeis.org/A081922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + (n-1)^2*a(n-2) - 4*(n-1)*(n-2)*a(n-3) with
 *            a(0)=1, a(1)=4, a(2)=17
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)17;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)4;   /* a(n-2) */
    seq_t a2 = (seq_t)17;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t m1 = (seq_t)(i - 1);   /* (i-1) as seq_t */
        seq_t m2 = (seq_t)(i - 2);   /* (i-2) as seq_t */

        /* (i-1)^2 * a(n-2) */
        seq_t term1 = m1 * m1 * a1;

        /* 4*(i-1)*(i-2) * a(n-3) */
        seq_t term2 = (seq_t)4 * m1 * m2 * a0;

        seq_t new_val = (seq_t)4 * a2 + term1 - term2;

        /* shift window: a(n-3)←a(n-2), a(n-2)←a(n-1), a(n-1)←new_val */
        a0 = a1;
        a1 = a2;
        a2 = new_val;
    }

    return a2;
}
