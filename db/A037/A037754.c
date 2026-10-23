/*
 * A037754  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,3,1,0.
 * https://oeis.org/A037754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1) - 8*a(n-2) + 8*a(n-3) - 7*a(n-4);
   Initial values: a(1)=2, a(2)=17, a(3)=120, a(4)=840 */
seq_t a(idx_t n)
{
    /* values for n < 5 */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)17;
    if (n == 3) return (seq_t)120;
    if (n == 4) return (seq_t)840;

    /* iterative computation */
    seq_t a1 = (seq_t)2;   /* a(n-4) */
    seq_t a2 = (seq_t)17;  /* a(n-3) */
    seq_t a3 = (seq_t)120; /* a(n-2) */
    seq_t a4 = (seq_t)840; /* a(n-1) */
    seq_t curr;

    for (idx_t i = 5; i <= n; ++i) {
        curr = (seq_t)8 * a4 - (seq_t)8 * a3 + (seq_t)8 * a2 - (seq_t)7 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = curr;
    }
    return a4;
}
