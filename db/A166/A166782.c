/*
 * A166782  Number of n X 3 1..3 arrays containing at least one of each value,
 *          all equal values connected, and rows considered as a single number
 *          in nondecreasing order.
 * https://oeis.org/A166782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (11*n^6 + 57*n^5 + 545*n^4 + 285*n^3 + 1424*n^2 - 1782*n
 *            + 540)/180
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use signed __int128 to avoid overflow in intermediate stages */
    signed __int128 nn   = (signed __int128)n;
    signed __int128 nn2  = nn * nn;                 /* n^2 */
    signed __int128 nn3  = nn2 * nn;                /* n^3 */
    signed __int128 nn4  = nn2 * nn2;               /* n^4 */
    signed __int128 nn5  = nn4 * nn;                /* n^5 */
    signed __int128 nn6  = nn3 * nn3;               /* n^6 */

    signed __int128 val =
          (__int128)11   * nn6
        + (__int128)57   * nn5
        + (__int128)545  * nn4
        + (__int128)285  * nn3
        + (__int128)1424 * nn2
        - (__int128)1782 * nn
        + (__int128)540;

    val /= 180;  /* exact division, numerator always divisible by 180 */

    return (seq_t)val;
}
