/*
 * A068245  1/6 the number of colorings of a 4 X 4 rhombic- or staggered-
 *          hexagonal array with n colors.
 * https://oeis.org/A068245
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^16 -33*n^15 +510*n^14 -4898*n^13 +32703*n^12
 *            -160859*n^11 +602408*n^10 -1749715*n^9 +3975561*n^8 -7068408*n^7
 *            +9755858*n^6 -10265148*n^5 +7968348*n^4 -4304712*n^3
 *            +1445104*n^2 -226720*n)/6
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 19, u128 n <= 262,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is undefined; return 0. */
    if (n < 3)
        return 0;

    /* The polynomial has degree 16, so we evaluate it with Horner's
       method.  For seq_t sizes up to 64‑bit we use an intermediate
       __int128 to avoid overflow during the calculation.  For larger
       seq_t we rely on big‑integer arithmetic. */
    if (sizeof(seq_t) <= 8) {
        __int128 r = 1;                     /* coefficient of n^16 */
        r = r * (__int128)n - 33;           /* n^15 term */
        r = r * (__int128)n + 510;
        r = r * (__int128)n - 4898;
        r = r * (__int128)n + 32703;
        r = r * (__int128)n - 160859;
        r = r * (__int128)n + 602408;
        r = r * (__int128)n - 1749715;
        r = r * (__int128)n + 3975561;
        r = r * (__int128)n - 7068408;
        r = r * (__int128)n + 9755858;
        r = r * (__int128)n - 10265148;
        r = r * (__int128)n + 7968348;
        r = r * (__int128)n - 4304712;
        r = r * (__int128)n + 1445104;
        r = r * (__int128)n - 226720;       /* coefficient of n^1 */
        r = r * (__int128)n;                 /* add the zero constant term */
        return (seq_t)(r / 6);
    } else {
        seq_t r = 1;                         /* coefficient of n^16 */
        r = r * n - 33;                      /* n^15 term */
        r = r * n + 510;
        r = r * n - 4898;
        r = r * n + 32703;
        r = r * n - 160859;
        r = r * n + 602408;
        r = r * n - 1749715;
        r = r * n + 3975561;
        r = r * n - 7068408;
        r = r * n + 9755858;
        r = r * n - 10265148;
        r = r * n + 7968348;
        r = r * n - 4304712;
        r = r * n + 1445104;
        r = r * n - 226720;                  /* coefficient of n^1 */
        r = r * n;                           /* add the zero constant term */
        r = r / 6;
        return r;
    }
}
