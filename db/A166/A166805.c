/*
 * A166805  Number of n X 4 1..2 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          nondecreasing order.
 * https://oeis.org/A166805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)*(n+2)*(n+3)*(n+4))/24 - 2
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 432, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* offset is 1; behaviour for n<1 is irrelevant */

    seq_t t1 = (seq_t)(n + 1);
    seq_t t2 = (seq_t)(n + 2);
    seq_t t3 = (seq_t)(n + 3);
    seq_t t4 = (seq_t)(n + 4);
    seq_t prod = t1;

    prod *= t2;
    prod *= t3;
    prod *= t4;
    prod /= (seq_t)24;
    prod -= (seq_t)2;

    return prod;
}
