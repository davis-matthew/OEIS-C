/*
 * A027481  Second subdiagonal of triangle A027477, constructed from the
 *          Stirling numbers of the first kind.
 * https://oeis.org/A027481
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*C(n+1,6) + 54*C(n+2,6) + 23*C(n+3,6) where C(m,6) =
 *            m*(m-1)*(m-2)*(m-3)*(m-4)*(m-5)/720 (C(m,6)=0 if m<6)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t binom6(seq_t m)
{
    if (m < 6) return 0;
    seq_t r = 1;
    /* compute r = product_{i=1..6} (m-6+i)/i  */
    r = r * (m - 5) / 1; /* term i=1 */
    r = r * (m - 4) / 2; /* term i=2 */
    r = r * (m - 3) / 3; /* term i=3 */
    r = r * (m - 2) / 4; /* term i=4 */
    r = r * (m - 1) / 5; /* term i=5 */
    r = r * (m)     / 6; /* term i=6 */
    return r;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t m1 = (seq_t)n + 1;          /* m1 = n+1 */
    seq_t m2 = (seq_t)n + 2;          /* m2 = n+2 */
    seq_t m3 = (seq_t)n + 3;          /* m3 = n+3 */

    seq_t c1 = binom6(m1);
    seq_t c2 = binom6(m2);
    seq_t c3 = binom6(m3);

    /* a(n) = 13*c1 + 54*c2 + 23*c3 */
    return (c1 * 13) + (c2 * 54) + (c3 * 23);
}
