/*
 * A166816  Number of n X 2 1..5 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          increasing order.
 * https://oeis.org/A166816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (n^8+32*n^7+476*n^6+3038*n^5-1351*n^4-33082*n^3+46234*n^2-15348*n)/5040
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 42, u64 n <= 739, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* -----------  Use 128‑bit arithmetic for the intermediate computation  ----------- */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 n2   = nn * nn;          /* n^2  */
    unsigned __int128 n3   = n2 * nn;          /* n^3  */
    unsigned __int128 n4   = n2 * n2;          /* n^4  */
    unsigned __int128 n5   = n4 * nn;          /* n^5  */
    unsigned __int128 n6   = n3 * n3;          /* n^6  */
    unsigned __int128 n7   = n6 * nn;          /* n^7  */
    unsigned __int128 n8   = n4 * n4;          /* n^8  */

    unsigned __int128 numerator =
          n8
        + (unsigned __int128)32   * n7
        + (unsigned __int128)476  * n6
        + (unsigned __int128)3038 * n5
        - (unsigned __int128)1351 * n4
        - (unsigned __int128)33082* n3
        + (unsigned __int128)46234* n2
        - (unsigned __int128)15348* nn;

    /* Divide by 5040 (exact integer division) */
    unsigned __int128 result = numerator / (unsigned __int128)5040;

    /* Convert back to seq_t (will truncate if seq_t is 64‑bit) */
    return (seq_t)result;
}
