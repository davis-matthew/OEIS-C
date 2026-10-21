/*
 * A061005  (Nearest integer to n^6/36) / 2.
 * https://oeis.org/A061005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n^6 / 36) / 2 (Nearest integer to n^6/36, divided
 *            by 2)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for safe intermediate multiplication */
    unsigned __int128 x = (unsigned __int128)n * n; /* n^2 */
    x *= n;                                         /* n^3 */
    x *= x;                                         /* n^6 */
    unsigned __int128 rounded = (x + 18) / 36;       /* round to nearest integer */
    return (seq_t)(rounded / 2);                    /* final division by 2 */
}
