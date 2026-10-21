/*
 * A089422  Maximum cycle size in range [A014137(n-1)..A014138(n-1)] of
 *          permutation A082335/A082336 (and also of A082349/A082350, to be
 *          proved).
 * https://oeis.org/A089422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=3; a(n)=2*n-2 for n>=4
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special cases for the first four terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* For n >= 4: a(n) = 2*n - 2 */
    seq_t res = (seq_t)n * (seq_t)2;
    res -= (seq_t)2;
    return res;
}
