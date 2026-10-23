/*
 * A144414  a(n) = 2*(4^n - 1)/3 - n.
 * https://oeis.org/A144414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(4^n - 1)/3 - n Equivalent recurrence: a(1)=1, a(2)=8,
 *            a(3)=39, a(n) = 6*a(n-1) - 9*a(n-2) + 4*a(n-3) (n ≥ 4)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – the value is irrelevant. */
    if (n < 1)
        return 0;

    /* Handle the first three terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)39;

    /* Iteratively evaluate the recurrence.  The values a(n-3), a(n-2)
       and a(n-1) are kept in `a1`, `a2` and `a3` respectively. */
    seq_t a1 = (seq_t)1;   /* a(n‑3)  */
    seq_t a2 = (seq_t)8;   /* a(n‑2)  */
    seq_t a3 = (seq_t)39;  /* a(n‑1)  */
    seq_t an = a3;         /* will hold a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 * (seq_t)6 - a2 * (seq_t)9 + a1 * (seq_t)4;
        /* Shift the window forward. */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;  /* a3 holds a(n) after the loop */
}
