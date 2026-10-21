/*
 * A225690  Number of Dyck paths of semilength n avoiding the pattern U^3 D^3
 *          U D.
 * https://oeis.org/A225690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n*n-15*n+6)/2 for n ≥ 4; initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=5
 * Range:     u8 n <= 7, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    seq_t nn   = (seq_t)n;
    seq_t term1 = (seq_t)5 * nn * nn;           /* 5*n^2   */
    seq_t term2 = (seq_t)15 * nn;               /* 15*n    */
    return (term1 - term2 + (seq_t)6) / (seq_t)2;
}
