/*
 * A060533  Number of homeomorphically irreducible multigraphs (or
 *          series-reduced multigraphs or multigraphs without nodes of degree
 *          2) on 3 labeled nodes.
 * https://oeis.org/A060533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)*(n+2))/2 - 9 for n > 4; base cases: a(0)=1,
 *            a(1)=3, a(2)=0, a(3)=3, a(4)=9
 * Range:     u8 n <= 21, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)3;
        case 2: return (seq_t)0;
        case 3: return (seq_t)3;
        case 4: return (seq_t)9;
        default: {
            seq_t tn = (seq_t)n;
            return ((tn + (seq_t)1) * (tn + (seq_t)2) / (seq_t)2) - (seq_t)9;
        }
    }
}
