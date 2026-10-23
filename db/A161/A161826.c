/*
 * A161826  Number of maximal vertex-independent sets in the hypergraph with
 *          nodes V = {1, 2, ..., n} and "edges" consisting of the triples
 *          (X,Y,Z) with X<Y<Z and X+Y=Z.
 * https://oeis.org/A161826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(2k) = 1 for k ≥ 5 (i.e., n ≥ 10 even), 
 *          a(2k+1) = 4 for k ≥ 5 (i.e., n ≥ 11 odd), 
 *          with the initial values below:
 *          a(1)=1, a(2)=1, a(3)=3, a(4)=2, a(5)=6,
 *          a(6)=1, a(7)=6, a(8)=1, a(9)=5, a(10)=1.
 */
seq_t a(idx_t n)
{
    /* Handle the explicitly defined initial segment */
    if (n <= 10) {
        switch (n) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)1;
        case 3: return (seq_t)3;
        case 4: return (seq_t)2;
        case 5: return (seq_t)6;
        case 6: return (seq_t)1;
        case 7: return (seq_t)6;
        case 8: return (seq_t)1;
        case 9: return (seq_t)5;
        case 10: return (seq_t)1;
        }
    }

    /* For n ≥ 11, the pattern alternates 1 for even, 4 for odd */
    if ((n & (seq_t)1) == 0)
        return (seq_t)1;
    else
        return (seq_t)4;
}
