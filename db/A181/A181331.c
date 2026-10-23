/*
 * A181331  Number of 0's in the top rows of all 2-compositions of n.
 * https://oeis.org/A181331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=5, a(3)=23, a(4)=99 For n≥5: a(n) =
 *            8*a(n-1) - 20*a(n-2) + 16*a(n-3) - 4*a(n-4)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A181331 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)23;
    if (n == 4) return (seq_t)99;

    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)5;   /* a(n-2) */
    seq_t a3 = (seq_t)23;  /* a(n-1) */
    seq_t a4 = (seq_t)99;  /* a(n  ) */

    for (idx_t i = 5; i <= n; ++i)
    {
        /* next term: 8*a4 - 20*a3 + 16*a2 - 4*a1 */
        seq_t next = (seq_t)8 * a4
                   - (seq_t)20 * a3
                   + (seq_t)16 * a2
                   - (seq_t)4  * a1;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
