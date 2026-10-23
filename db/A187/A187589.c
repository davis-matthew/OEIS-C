/*
 * A187589  Number of 6-step E, S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 714*n^2 - 3888*n + 5104 for n>4 Initial values: a(1)=0,
 *            a(2)=0, a(3)=104, a(4)=1069, a(5)=3514 Recurrence for n>7: a(n)
 *            = 3*a(n-1) - 3*a(n-2) + a(n-3) (unused here)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)5) {
        switch ((unsigned)n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)104;
            case 4: return (seq_t)1069;
            case 5: return (seq_t)3514;
            default: return (seq_t)0; /* unreachable for n<=5 */
        }
    }
    seq_t nn = (seq_t)n;
    /* compute 714*n^2 - 3888*n + 5104 */
    seq_t res = (seq_t)714 * nn * nn
                - (seq_t)3888 * nn
                + (seq_t)5104;
    return res;
}
