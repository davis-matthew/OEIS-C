/*
 * A209539  Number of nX4 array permutations with every element making zero or
 *          one right-handed knight moves (out 2, right 1).
 * https://oeis.org/A209539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) for n>6; a(1)=1, a(2)=4, a(3)=72, * a(4)=729,
 *            a(5)=6480, a(6)=63504
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle first 6 terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)72;
    if (n == 4) return (seq_t)729;
    if (n == 5) return (seq_t)6480;
    if (n == 6) return (seq_t)63504;
    if (n < 1)   return (seq_t)0;   /* undefined for n < 1 */

    /* for n > 6, multiply by 10 (n-6) times */
    seq_t val = (seq_t)63504;
    idx_t k = n - 6;
    for (idx_t i = 0; i < k; ++i) {
        val *= 10;
    }
    return val;
}
