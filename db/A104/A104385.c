/*
 * A104385  Number of distinct partitions of triangular numbers n*(n+1)/2 into
 *          3 parts for n>=1.
 * https://oeis.org/A104385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round((3*n^4 + 6*n^3 - 33*n^2 - 36*n + 110)/144)
 * Range:     u8 n <= 10, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* use a wide type for intermediate calculations to avoid overflow */
    unsigned __int128 N = n;
    unsigned __int128 num = 3*N*N*N*N
                          + 6*N*N*N
                          - 33*N*N
                          - 36*N
                          + 110;

    unsigned __int128 q128 = num / 144u;
    unsigned __int128 r128 = num % 144u;
    seq_t q = (seq_t)q128;          /* q fits into seq_t for the n-range tested */

    if (r128 >= 72u)                 /* round to nearest integer */
        return q + 1;
    else
        return q;
}
