/*
 * A028231  From hexagons in a circle problem.
 * https://oeis.org/A028231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=22, a(2)=313, 
   a(n)=15*a(n-1)-15*a(n-2)+a(n-3)  (OEIS A028231) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)313;

    seq_t a0 = (seq_t)1;  /* a(n-3) */
    seq_t a1 = (seq_t)22; /* a(n-2) */
    seq_t a2 = (seq_t)313;/* a(n-1) */
    seq_t an = a2;        /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i)
    {
        an = (seq_t)15 * a2 - (seq_t)15 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}
