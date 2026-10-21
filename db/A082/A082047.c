/*
 * A082047  Diagonal sums of number array A082046.
 * https://oeis.org/A082047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^5 + 15*n^3 + 14*n + 30) / 30
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 166, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* arbitrary‑precision (C++) implementation */
    seq_t nn = n;
    seq_t n2   = nn * nn;        /* n^2   */
    seq_t n3   = n2 * nn;        /* n^3   */
    seq_t n5   = n3 * n2;        /* n^5   */
    seq_t num  = n5 + 15 * n3 + 14 * nn + 30;
    return num / 30;
#else
    /* built‑in integer (C) implementation using 128‑bit intermediate */
    unsigned __int128 nn = n;
    unsigned __int128 n2 = nn * nn;        /* n^2   */
    unsigned __int128 n3 = n2 * nn;        /* n^3   */
    unsigned __int128 n5 = n3 * n2;        /* n^5   */
    unsigned __int128 num = n5 + 15 * n3 + 14 * nn + 30;
    return (seq_t)(num / 30);
#endif
}
