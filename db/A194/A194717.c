/*
 * A194717  Number of n-ary words beginning with the first character of the
 *          alphabet, that can be built by inserting five doublets into the
 *          initially empty word.
 * https://oeis.org/A194717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 8*(n-1) + 27*(n-1)^2 + 48*(n-1)^3 + 42*(n-1)^4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 101, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A194717: Number of n-ary words beginning with the first character of the alphabet,   */
/*  that can be built by inserting five doublets into the initially empty word.                          */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Working with m = n-1 as a seq_t to keep all arithmetic in seq_t. */
    seq_t m = (seq_t)(n - 1);

    /* Compute the polynomial 1 + 8m + 27m^2 + 48m^3 + 42m^4. */
    seq_t m2 = m * m;          /* m^2 */
    seq_t m3 = m2 * m;         /* m^3 */
    seq_t m4 = m3 * m;         /* m^4 */

    seq_t result = 1;
    result += 8u * m;
    result += 27u * m2;
    result += 48u * m3;
    result += 42u * m4;

    return result;
}
