/*
 * A195003  Number of lower triangles of a 7 X 7 0..n array with each element
 *          differing from all of its horizontal and vertical neighbors by
 *          one.
 * https://oeis.org/A195003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1957392*n - 6016308 for n>10 a(1) = 2, a(2) = 69632, a(3)
 *            = 641376, a(4) = 2059822, a(5) = 3816148, a(6) = 5740166, a(7) =
 *            7686580, a(8) = 9643120, a(9) = 11600228, a(10) = 13557614
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Values for n = 1 … 10 taken directly from the OEIS data. */
    if (n <= 10U) {
        static const seq_t tbl[11] = {
            0ULL,         /* tbl[0] is unused, placeholder */
            2ULL,         /* a(1) */
            69632ULL,     /* a(2) */
            641376ULL,    /* a(3) */
            2059822ULL,   /* a(4) */
            3816148ULL,   /* a(5) */
            5740166ULL,   /* a(6) */
            7686580ULL,   /* a(7) */
            9643120ULL,   /* a(8) */
            11600228ULL,  /* a(9) */
            13557614ULL   /* a(10) */
        };
        return tbl[(idx_t)n];
    }

    /* Linear formula for n > 10 */
    return (seq_t)n * (seq_t)1957392ULL - (seq_t)6016308ULL;
}
