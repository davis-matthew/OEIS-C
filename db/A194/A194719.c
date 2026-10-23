/*
 * A194719  Number of n-ary words beginning with the first character of the
 *          alphabet, that can be built by inserting seven doublets into the
 *          initially empty word.
 * https://oeis.org/A194719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 12*(n-1) + 65*(n-1)^2 + 208*(n-1)^3 + 429*(n-1)^4 * +
 *            572*(n-1)^5 + 429*(n-1)^6 for n > 0 * a(0) = 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 592, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* m = n-1 */
    seq_t m = (seq_t)(n - 1);

    /* Powers of m: m, m^2, ..., m^6 */
    seq_t m1 = m;          /* m */
    seq_t m2 = m1 * m;     /* m^2 */
    seq_t m3 = m2 * m;     /* m^3 */
    seq_t m4 = m3 * m;     /* m^4 */
    seq_t m5 = m4 * m;     /* m^5 */
    seq_t m6 = m5 * m;     /* m^6 */

    /* Compute result using the polynomial */
    seq_t result = 1
        + 12 * m1
        + 65 * m2
        + 208 * m3
        + 429 * m4
        + 572 * m5
        + 429 * m6;

    return result;
}
