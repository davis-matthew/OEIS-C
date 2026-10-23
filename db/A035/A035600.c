/*
 * A035600  Number of points of L1 norm 6 in cubic lattice Z^n.
 * https://oeis.org/A035600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^6 + 40*n^4 + 46*n^2)/45
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the compiler supports a 128‑bit integer, use it for intermediate
       calculations to avoid overflow for larger n.  For builds where
       seq_t itself is 128 bits or larger, the conversion from a 128 bit
       type is well defined. */
#if defined(__SIZEOF_INT128__)
    typedef unsigned __int128 u128;
    u128 nn = (u128)n * n;          /* n² */
    u128 n4 = nn * nn;              /* n⁴ */
    u128 n6 = n4 * nn;              /* n⁶ */
    u128 num = 4 * n6 + 40 * n4 + 46 * nn;
    return (seq_t)(num / 45);
#else
    seq_t nn = (seq_t)n * (seq_t)n; /* n² */
    seq_t n4 = nn * nn;             /* n⁴ */
    seq_t n6 = n4 * nn;             /* n⁶ */
    seq_t num = 4 * n6 + 40 * n4 + 46 * nn;
    return num / 45;
#endif
}
