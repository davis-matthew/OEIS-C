/*
 * A047663  Row 6 of square array defined in A047662.
 * https://oeis.org/A047663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/45) * (2*n^5 + 6*n^4 + 35*n^3 + 60*n^2 + 98*n + 69)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 67, u64 n <= 2730,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using 128‑bit intermediate to avoid overflow in 64‑bit builds. */
    unsigned __int128 nn = n;
    unsigned __int128 n2 = nn * nn;           /* n^2 */
    unsigned __int128 n3 = n2 * nn;           /* n^3 */
    unsigned __int128 n4 = n3 * nn;           /* n^4 */
    unsigned __int128 n5 = n4 * nn;           /* n^5 */

    unsigned __int128 poly = 2 * n5 + 6 * n4 + 35 * n3 + 60 * n2 + 98 * nn + 69;

    unsigned __int128 prod = nn * poly;
    unsigned __int128 res = prod / 45;        /* exact division */

    return (seq_t)res;
}
