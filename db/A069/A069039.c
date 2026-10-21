/*
 * A069039  Expansion of g.f. x*(1+x)^5/(1-x)^7.
 * https://oeis.org/A069039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(2*n^4+20*n^2+23)/45
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 67, u64 n <= 2731,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128-bit integer for intermediate calculations to avoid overflow */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;           /* n^2 */
    unsigned __int128 n4 = n2 * n2;           /* n^4 */
    unsigned __int128 expr = 2 * n4 + 20 * n2 + 23;
    unsigned __int128 num  = n2 * expr;       /* n^2 * expr */
    unsigned __int128 res  = num / 45;        /* exact division */
    return (seq_t)res;
}
