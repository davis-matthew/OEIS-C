/*
 * A134456  The sequence of the initial letters of the days of the week (in
 *          English) in ASCII format, starting with a Sunday.
 * https://oeis.org/A134456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, less, easy
 * Formula:   a(n) = {83,77,84,87,84,70,83}[n % 7] (period 7, Sunday first)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 7;
    switch (r) {
    case 0: return 83; /* 'S' Sunday   */
    case 1: return 77; /* 'M' Monday   */
    case 2: return 84; /* 'T' Tuesday  */
    case 3: return 87; /* 'W' Wednesday*/
    case 4: return 84; /* 'T' Thursday */
    case 5: return 70; /* 'F' Friday   */
    case 6: return 83; /* 'S' Saturday */
    }
    return 0; /* unreachable, keeps compiler happy */
}
