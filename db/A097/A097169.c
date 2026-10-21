/*
 * A097169  a(n) = Sum_{k=0..n} C(floor((n+1)/2),floor((k+1)/2)) * 3^k.
 * https://oeis.org/A097169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} C( floor((n+1)/2), floor((k+1)/2) ) * 3^k
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*               a(n) = a(n‑1) + 19*a(n‑2) – 19*a(n‑3) – 90*a(n‑4) + 90*a(n‑5) */
/* Initial values: a(0)=1, a(1)=4, a(2)=13, a(3)=52, a(4)=133 */

seq_t a(idx_t n)
{
    /* Initial known values */
    static const seq_t init[5] = { (seq_t)1, (seq_t)4, (seq_t)13, (seq_t)52, (seq_t)133 };

    /* If needed, return one of the initial values */
    if (n < 5)
        return init[n];

    /* Variables to hold the last five computed terms */
    seq_t a0 = init[0];   /* a(n-5) */
    seq_t a1 = init[1];   /* a(n-4) */
    seq_t a2 = init[2];   /* a(n-3) */
    seq_t a3 = init[3];   /* a(n-2) */
    seq_t a4 = init[4];   /* a(n-1) */
    seq_t an;              /* current a(n) */

    /* Compute terms from 5 up to n using the recurrence */
    for (idx_t i = 5; i <= n; ++i)
    {
        an = a4 + (seq_t)19 * a3 - (seq_t)19 * a2 - (seq_t)90 * a1 + (seq_t)90 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return an;
}
