/*
 * A163944  Fourth left hand column of triangle A163940.
 * https://oeis.org/A163944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (10*n^2 +107*n^3 +61*n^4 +13*n^5 +n^6)/48
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 74, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 128‑bit integer for intermediate computation to avoid overflow.
       __int128 is available with GCC/Clang and works on C11 and C++17. */
    __int128 nn  = (unsigned long long)n;    /* promote index to 128‑bit */
    __int128 p2  = nn * nn;                  /* n^2 */
    __int128 p3  = p2 * nn;                  /* n^3 */
    __int128 p4  = p3 * nn;                  /* n^4 */
    __int128 p5  = p4 * nn;                  /* n^5 */
    __int128 p6  = p5 * nn;                  /* n^6 */

    __int128 num = (10 * p2) + (107 * p3) + (61 * p4) + (13 * p5) + p6;
    seq_t    res = (seq_t)(num / 48);         /* final division */
    return res;
}
