/*
 * A021454  Expansion of 1/((1-x)(1-3x)(1-5x)(1-9x)).
 * https://oeis.org/A021454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=18, a(2)=220, a(3)=2310;
   for n≥4: a(n)=18*a(n-1)-104*a(n-2)+222*a(n-3)-135*a(n-4). */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)18;
    if (n == 2) return (seq_t)220;
    if (n == 3) return (seq_t)2310;

    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)18;  /* a(n-3) */
    seq_t a2 = (seq_t)220; /* a(n-2) */
    seq_t a3 = (seq_t)2310;/* a(n-1) */
    seq_t an = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)18 * a3
           - (seq_t)104 * a2
           + (seq_t)222 * a1
           - (seq_t)135 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
