/*
 * A219579  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or vertical neighbors in a random,
 *          but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..3 2 X n array.
 * https://oeis.org/A219579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 178, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A219579
 *
 * Formula: a(n) = (3*n^5 - 25*n^4 + 395*n^3 - 455*n^2 + 322*n - 120)/120,
 *           a(1) = 4
 *
 * The polynomial is valid for all n > 1.  For n = 1 the sequence is defined
 * to be 4.  All arithmetic is performed in an intermediate 128‑bit signed
 * integer to avoid overflow for the values that fit in a 64‑bit result.
 */
seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 1) return (seq_t)4;

    /* Use 128‑bit arithmetic for safe intermediate results. */
    __int128 nn = ( __int128 ) n;

    /* Compute the numerator: 3*n^5 - 25*n^4 + 395*n^3 - 455*n^2 + 322*n - 120 */
    __int128 n2 = nn * nn;          /* n^2 */
    __int128 n3 = n2 * nn;          /* n^3 */
    __int128 n4 = n3 * nn;          /* n^4 */
    __int128 n5 = n4 * nn;          /* n^5 */

    __int128 numerator =
        +3   * n5
        -25  * n4
        +395 * n3
        -455 * n2
        +322 * nn
        -120;

    /* Divide by 120 to get the final term */
    return (seq_t)(numerator / 120);
}
