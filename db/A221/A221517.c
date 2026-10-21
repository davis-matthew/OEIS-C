/*
 * A221517  Number of 0..n arrays of length 6 with each element differing from
 *          at least one neighbor by 2 or more, starting with 0.
 * https://oeis.org/A221517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 - 20*n^3 + 78*n^2 - 146*n + 125 for n > 4 Initial
 *            values: a(1)=0, a(2)=5, a(3)=89, a(4)=532 This implementation
 *            works for any seq_t chosen by the build (uint8_t … unsigned
 *            __int128, or arbitrary‑precision class). It is O(1) and works in
 *            any call order
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the four known small terms explicitly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)89;
    if (n == 4) return (seq_t)532;

    /* For n >= 5 use the closed polynomial formula. */
    seq_t nn = (seq_t)n;          /* cast once to the target type   */
    seq_t val = nn * nn * nn * nn * nn;   /* n^5 */
    seq_t n3 = nn * nn * nn;                         /* n^3 */
    val -= (seq_t)20 * n3;
    seq_t n2 = nn * nn;                         /* n^2 */
    val += (seq_t)78 * n2;
    val -= (seq_t)146 * nn;
    val += (seq_t)125;
    return val;
}
