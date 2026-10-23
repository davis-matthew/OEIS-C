/*
 * A039824  Number of different coefficient values in expansion of Product
 *          (1+q^1+q^3...+q^(2i-1)), i=1 to n.
 * https://oeis.org/A039824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - 3 for n>=7; Initial values: a(1)=1, a(2)=2, a(3)=4,
 *            a(4)=11, a(5)=20, a(6)=31
 * Range:     u8 n <= 16, u16 n <= 180, u32 n <= 180, u64 n <= 180,
 *            u128 n <= 180, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n == 0) return 0;                /* undefined below offset, return 0 */

    static const seq_t init[6] = {1, 2, 4, 11, 20, 31};

    if (n <= 6) return init[n - 1];

    seq_t nn = (seq_t)n;
    return nn * nn - (seq_t)3;
}
