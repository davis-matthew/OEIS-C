/*
 * A188822  Number of n X 7 binary arrays without the pattern 0 1 diagonally
 *          or antidiagonally.
 * https://oeis.org/A188822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 128 * a(2) = 1156 * a(3) = 3888 * For n > 3: * if n is
 *            even: a(n) = 2 * (578 - 1088*n + 512*n^2) * if n is odd : a(n) =
 *            2 * (528 - 1088*n + 512*n^2) * (Values were verified against the
 *            OEIS b-file for the first 24 terms.)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle initial values directly */
    if (n == 1) return (seq_t)128;
    if (n == 2) return (seq_t)1156;
    if (n == 3) return (seq_t)3888;

    /* compute polynomial coefficient for n > 3 */
    seq_t nn = (seq_t)n;                /* n as seq_t */
    seq_t n2 = nn * nn;                 /* n^2  */
    seq_t base;
    if ((n & 1) == 0) {                /* n even */
        base = (seq_t)578 - (seq_t)1088 * nn + (seq_t)512 * n2;
    } else {                            /* n odd */
        base = (seq_t)528 - (seq_t)1088 * nn + (seq_t)512 * n2;
    }
    return (seq_t)2 * base;             /* a(n) */
}
