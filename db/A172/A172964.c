/*
 * A172964  Number of ways to place 2 nonattacking knights on an n X n
 *          cylindrical board.
 * https://oeis.org/A172964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3 - 9*n + 12)/2 (for n>=5); a(1)=0, a(2)=4, a(3)=18,
 *            a(4)=92
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 264, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4)
    {
        switch (n)
        {
            case 1: return (seq_t)0;
            case 2: return (seq_t)4;
            case 3: return (seq_t)18;
            case 4: return (seq_t)92;
        }
    }
    seq_t nn   = (seq_t)n;
    seq_t n3   = nn * nn * nn;                  /* n^3  */
    seq_t term = n3 - (seq_t)9 * nn + (seq_t)12; /* n^3 - 9n + 12 */
    return (nn * term) / (seq_t)2;
}
