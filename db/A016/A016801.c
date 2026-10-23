/*
 * A016801  Expansion of g.f. 1/((1 - 3*x)*(1 - 4*x)*(1 - 7*x)).
 * https://oeis.org/A016801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 61*a(n-2) + 84*a(n-3); with a(0) = 1, a(1) =
 *            14, a(2) = 135
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)135;

    /* iterative recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)14;  /* a(n-2) */
    seq_t a2 = (seq_t)135; /* a(n-1) */
    seq_t an;

    /* compute from 3 up to n */
    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)14 * a2 - (seq_t)61 * a1 + (seq_t)84 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
