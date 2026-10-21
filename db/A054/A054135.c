/*
 * A054135  a(n) = T(n,1), array T as in A054134.
 * https://oeis.org/A054135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^(n-2) - 1, a(1) = 2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  For n = 2, 3, 4, … the value is 5*2^(n-2) - 1. */
/*  The function accepts any index n >= 1. */
/*  seq_t and idx_t are defined by the build system. */

seq_t a(idx_t n)
{
    /* Offset 1:  a(1) = 2 */
    if (n == 1)
        return (seq_t)2;

    /* Compute 2^(n-2) by repeated doubling */
    seq_t pow2 = (seq_t)1;
    /* idx_t is unsigned; n>=2 here, so (n-2) is valid */
    for (idx_t i = 0; i < n - 2; ++i)
        pow2 = pow2 * (seq_t)2;

    /* 5*pow2 - 1 */
    return (seq_t)5 * pow2 - (seq_t)1;
}
