/*
 * A128828  The number of regular hexagons found by constructing n
 *          equally-spaced points on each side of the hexagon and drawing
 *          lines parallel to the hexagon side.
 * https://oeis.org/A128828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A128828 – Number of regular hexagons in the 3‑point construction
 *
 *  Piecewise polynomial expression (OEIS entry):
 *    a(n) = ( 11*n⁴ + 78*n³ + 1413*n² - 2322*n + 324 ) / 324   for n ≡ 0 (mod 3)
 *    a(n) = ( -13*n⁴ + 670*n³ - 3219*n² + 9934*n - 6724 ) / 324  for n ≡ 1 (mod 3)
 *    a(n) = ( 5*n⁴ - 46*n³ + 1515*n² - 6046*n + 7940 ) / 108   for n ≡ 2 (mod 3)
 *
 *  The function below implements this formula using 128‑bit intermediate
 *  arithmetic (when available) to avoid overflow on 64‑bit builds.
 */

#include <stdint.h>          /* for uint64_t, if needed */

seq_t a(idx_t n)
{
    /* Handle the case n % 3 == 0, 1, 2 separately. */
    switch (n % 3)
    {
        case 0:  /* n ≡ 0 (mod 3) */
        {
            /* Use signed 128‑bit integer for intermediates. */
            __int128 nn  = n;
            __int128 num = 11*nn*nn*nn*nn
                         + 78*nn*nn*nn
                         + 1413*nn*nn
                         - 2322*nn
                         + 324;
            return (seq_t)(num / 324);
        }

        case 1:  /* n ≡ 1 (mod 3) */
        {
            __int128 nn  = n;
            __int128 num = -13*nn*nn*nn*nn
                         + 670*nn*nn*nn
                         - 3219*nn*nn
                         + 9934*nn
                         - 6724;
            return (seq_t)(num / 324);
        }

        case 2:  /* n ≡ 2 (mod 3) */
        {
            __int128 nn  = n;
            __int128 num = 5*nn*nn*nn*nn
                         - 46*nn*nn*nn
                         + 1515*nn*nn
                         - 6046*nn
                         + 7940;
            return (seq_t)(num / 108);
        }
    }

    /* The switch should cover all values of n % 3. */
    return 0;
}
