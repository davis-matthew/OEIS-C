/*
 * A089425  Least common multiple of all cycle sizes (and also the maximum
 *          cycle size) in range [A014137(n-1)..A014138(n-1)] of permutation
 *          A082351/A082352.
 * https://oeis.org/A089425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0,1,2; a(3) = 3; a(n) = 3*(n-1) for n ≥ 4
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;
    if (n == 3)
        return (seq_t)3;
    return (seq_t)3 * ((seq_t)n - (seq_t)1);
}
