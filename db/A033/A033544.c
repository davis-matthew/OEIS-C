/*
 * A033544  Wiener number of n-hexagonal triangle.
 * https://oeis.org/A033544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(4*n^3+36*n^2+79*n+16)/10
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 99, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the 128‑bit type if the target type fits into 64 bits to avoid
     * overflow before the final division.  For larger types the native
     * arithmetic suffices. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 nn = n;
        unsigned __int128 part1 = nn * (nn + 1);               /* n*(n+1) */
        unsigned __int128 part2 = 4 * nn * nn * nn              /* 4*n^3   */
                                + 36 * nn * nn                 /* 36*n^2  */
                                + 79 * nn                      /* 79*n    */
                                + 16;                          /* +16     */
        unsigned __int128 prod = part1 * part2;                 /* numerator */
        prod /= 10;                                            /* divide by 10 */
        return (seq_t) prod;
    } else {
        seq_t nn = (seq_t) n;
        seq_t part1 = nn * (nn + 1);                            /* n*(n+1) */
        seq_t part2 = 4 * nn * nn * nn                          /* 4*n^3   */
                    + 36 * nn * nn                               /* 36*n^2  */
                    + 79 * nn                                     /* 79*n    */
                    + 16;                                         /* +16     */
        seq_t prod = part1 * part2;                             /* numerator */
        prod /= 10;                                            /* divide by 10 */
        return prod;
    }
}
