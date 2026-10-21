/*
 * A095977  Expansion of g.f. 2*x / ((1+x)^2*(1-2*x)^2).
 * https://oeis.org/A095977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/27)*((3*n+2)*2^(n+2) - (6*n+8)*(-1)^n)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=4, a(3)=14, a(4)=32,
   a(n)=2*a(n-1)+3*a(n-2)-4*a(n-3)-4*a(n-4)  for n≥5 */

seq_t a(idx_t n)
{
    /* offset is 1; ignore n==0 for safety */
    if (n == 0)
        return 0;

    /* base values for first four terms */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)14;
    if (n == 4) return (seq_t)32;

    /* iterate using the linear recurrence */
    seq_t a1 = (seq_t)2;   /* a(n-4) */
    seq_t a2 = (seq_t)4;   /* a(n-3) */
    seq_t a3 = (seq_t)14;  /* a(n-2) */
    seq_t a4 = (seq_t)32;  /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 5; i <= n; ++i)
    {
        an = (seq_t)2 * a4  /* 2*a(n-1) */
            + (seq_t)3 * a3 /* 3*a(n-2) */
            - (seq_t)4 * a2 /* -4*a(n-3) */
            - (seq_t)4 * a1;/* -4*a(n-4) */

        a1 = a2; a2 = a3; a3 = a4; a4 = an;
    }

    return an;
}
