/*
 * A030180  a(n) = (n^7 - n)/42.
 * https://oeis.org/A030180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^7 - n)/42
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 40, u64 n <= 964, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute n^7 - n, then divide by 42 */
    __int128 n128 = (__int128)n;
    __int128 n2   = n128 * n128;     /* n^2 */
    __int128 n4   = n2 * n2;         /* n^4 */
    __int128 n7   = n4 * n2 * n128;  /* n^7 */
    __int128 num  = n7 - n128;       /* n^7 - n */
    return (seq_t)(num / 42);
}
