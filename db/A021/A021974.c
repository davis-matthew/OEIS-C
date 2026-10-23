/*
 * A021974  Expansion of g.f. 1/((1-x)*(1-4*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A021974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 26*a(n-1) -217*a(n-2) +624*a(n-3) -432*a(n-4);
   Initial values: a(0)=1, a(1)=26, a(2)=459, a(3)=6916 */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return 1;
    if (n == 1) return 26;
    if (n == 2) return 459;
    if (n == 3) return 6916;

    /* a0 .. a3 hold a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = 1;     /* a(0) */
    seq_t a1 = 26;    /* a(1) */
    seq_t a2 = 459;   /* a(2) */
    seq_t a3 = 6916;  /* a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* use the recurrence; addition of positives first
           to avoid unsigned underflow (the term 26*a3 dominates). */
        next = 26 * a3;
        next += 624 * a1;
        next -= 217 * a2;
        next -= 432 * a0;

        /* shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
