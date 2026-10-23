/*
 * A173516  a(1)=1, a(n) = 3*n*a(n-1) + 1, n > 1.
 * https://oeis.org/A173516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*a(n-1)+1, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but harmless for n = 0 */

    seq_t val = 1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* Use explicit seq_t casts for the constants to work with
           arbitrary-precision or class-based seq_t in C++ builds. */
        val = val * (((seq_t)i) * (seq_t)3) + (seq_t)1;
    }

    return val;
}
