/*
 * A037631  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,3,0.
 * https://oeis.org/A037631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=23, a(3)=230, a(4)=2302,
   a(n) = 10*a(n-1) + a(n-3) - 10*a(n-4)  for n ≥ 5 */
seq_t a(idx_t n)
{
    /* return 0 for an undefined offset 0; valid indices start at 1 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)23;
    if (n == 3) return (seq_t)230;
    if (n == 4) return (seq_t)2302;

    /* keep only the last four values to compute the next one */
    seq_t a1 = (seq_t)2;    /* a(n-4) */
    seq_t a2 = (seq_t)23;   /* a(n-3) */
    seq_t a3 = (seq_t)230;  /* a(n-2) */
    seq_t a4 = (seq_t)2302; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t an = ((seq_t)10 * a4) + a2 - ((seq_t)10 * a1);
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
