/*
 * A089574  Column 4 of an array closely related to A083480. (Both arrays have
 *          shape sequence A083479).
 * https://oeis.org/A089574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(4*n^3+51*n^2+159*n+86)/120
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for index 0; OEIS offset is 1 */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 poly = (unsigned __int128)4 * nn * nn * nn
                           + (unsigned __int128)51 * nn * nn
                           + (unsigned __int128)159 * nn
                           + (unsigned __int128)86;
    unsigned __int128 val = nn * (nn + 1) * poly / 120;
    return (seq_t)val;
}
