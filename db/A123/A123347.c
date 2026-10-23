/*
 * A123347  Number of words of length n over the alphabet {1,2,3,4,5} such
 *          that 1 is not followed by an odd letter.
 * https://oeis.org/A123347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) for n>1, a(0) = 1, a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)5;

    seq_t a0 = (seq_t)1;    /* a(0) */
    seq_t a1 = (seq_t)5;    /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = 4 * a1 + 2 * a0; /* a(i) */
        a0 = a1;
        a1 = an;
    }
    return a1;
}
