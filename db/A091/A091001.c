/*
 * A091001  Number of walks of length n between adjacent nodes on the Petersen
 *          graph.
 * https://oeis.org/A091001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=0, a(1)=1, a(2)=0,
   a(n)=2*a(n-1)+5*a(n-2)-6*a(n-3)  for n≥3 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;

    seq_t a0 = (seq_t)0;   // a(n-3)
    seq_t a1 = (seq_t)1;   // a(n-2)
    seq_t a2 = (seq_t)0;   // a(n-1)
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = 2 * a2 + 5 * a1 - 6 * a0;   // compute a(i)
        a0 = a1;                         // shift window
        a1 = a2;
        a2 = an;
    }
    return a2;
}
