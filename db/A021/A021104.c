/*
 * A021104  Expansion of g.f. 1/((1 - x)*(1 - 2*x)*(1 - 4*x)*(1 - 12*x)).
 * https://oeis.org/A021104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 19, a(2) = 263, a(3) = 3311
   For n > 3: a(n) = 19*a(n-1) - 98*a(n-2) + 176*a(n-3) - 96*a(n-4) */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;
    if (n == 2) return (seq_t)263;
    if (n == 3) return (seq_t)3311;

    /* Use a sliding window of size 4 to store the last four values. */
    seq_t a0 = (seq_t)1;      /* a(n-4)  */
    seq_t a1 = (seq_t)19;     /* a(n-3)  */
    seq_t a2 = (seq_t)263;    /* a(n-2)  */
    seq_t a3 = (seq_t)3311;   /* a(n-1)  */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = 19 * a3 - 98 * a2 + 176 * a1 - 96 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
