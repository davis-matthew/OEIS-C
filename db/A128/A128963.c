/*
 * A128963  a(n) = (n^3 - n)*5^n.
 * https://oeis.org/A128963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n)*5^n Recurrence: a(1)=0, a(2)=150, a(3)=3000,
 *            a(4)=37500; a(n)=20*a(n-1)-150*a(n-2)+500*a(n-3)-625*a(n-4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)150;
    if (n == 3) return (seq_t)3000;
    if (n == 4) return (seq_t)37500;

    /* Keep the last four values of the sequence */
    seq_t a1 = (seq_t)0;      /* a(n-4) */
    seq_t a2 = (seq_t)150;    /* a(n-3) */
    seq_t a3 = (seq_t)3000;   /* a(n-2) */
    seq_t a4 = (seq_t)37500;  /* a(n-1) */

    /* Iterate using the recurrence until the desired n is reached */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = ((seq_t)20 * a4 + (seq_t)500 * a2)
                     - ((seq_t)150 * a3 + (seq_t)625 * a1);
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
