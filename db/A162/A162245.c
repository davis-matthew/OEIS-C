/*
 * A162245  Triangle T(n,m) = 6*m*n + 3*m + 3*n + 1 read by rows.
 * https://oeis.org/A162245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 6*c*r + 3*c + 3*r + 1 * where r is the smallest integer
 *            with r*(r+1)/2 >= n * and c = n - (r-1)*r/2 is the column index
 *            in the triangle
 * Range:     u8 n <= 26, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   _row_for
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t on C/C++ */
#include "../utils/_row_for.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* undefined, return zero for safety */
        return 0;
    idx_t r = _row_for(n);          /* row index (1‑based)  */
    idx_t prev = (r - 1) * r / 2;   /* number of terms before row r */
    idx_t c = n - prev;             /* column index (1‑based) */

    idx_t prod = 6 * c * r;           /* intermediate products < 2^64 for n up to 10^15 */
    idx_t add  = 3 * c + 3 * r + 1;   /* safe for 64‑bit */
    return (seq_t)(prod + add);
}
