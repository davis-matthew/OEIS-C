/*
 * A187732  Expansion of x/(x^4 - 13x^3 + 36x^2 - 13x + 1).
 * https://oeis.org/A187732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=13, a(3)=133,
   a(n)=13*a(n-1)-36*a(n-2)+13*a(n-3)-a(n-4)  */
seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)13;
    if (n == 3) return (seq_t)133;

    /* We only need the last four values to compute the next one. */
    seq_t a0 = (seq_t)0,   /* a(n-4) */
           a1 = (seq_t)1,   /* a(n-3) */
           a2 = (seq_t)13,  /* a(n-2) */
           a3 = (seq_t)133; /* a(n-1) */

    for (idx_t k = 4; k <= n; ++k) {
        seq_t next = 13 * a3 - 36 * a2 + 13 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
