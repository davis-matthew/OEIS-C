/*
 * A105698  Fixed point of morphism 1->{1, 4, 2, 1}, 2->{2, 1, 3, 2}, 3->{3,
 *          2, 4, 3}, 4->{4, 1, 3, 4}.
 * https://oeis.org/A105698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0, let k=n/4 and r=n%4;
   a(n)=M[a(k)][r] where M[1]={1,4,2,1}, M[2]={2,1,3,2},
   M[3]={3,2,4,3}, M[4]={4,1,3,4}. */

static const seq_t trans[5][4] = {
    {0,0,0,0},
    {1,4,2,1},
    {2,1,3,2},
    {3,2,4,3},
    {4,1,3,4}
};

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t parent = a(n / 4);
    return trans[(int)parent][n & 3];
}
