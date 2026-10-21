/*
 * A055261  Sums of two powers of 16.
 * https://oeis.org/A055261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   a(n) = 16^trinv(n) + 16^(n - trinv(n)) where trinv(n) =
 *            floor((sqrt(8*n-7)-1)/2) (inverse of triangular numbers)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 36, u64 n <= 136, u128 n <= 528,
 *            big any n
 * Helpers:   _floor_isqrt_u64, pow16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/pow16.h"

seq_t a(idx_t n) {
    if (n == 0)    /* should not happen for the sequence offset 1 */
        return 0;

    /* Compute the row index r  (0‑based) */
    uint64_t w   = 8ULL * n - 7ULL;             /* 8*n-7 fits in 64 bits for all n in usual range */
    uint64_t s   = _floor_isqrt_u64(w);         /* integer sqrt of 8*n-7 */
    idx_t   r    = (s - 1ULL) / 2ULL;           /* trinv(n)-1  */

    /* Position of n within this row (0‑based)  */
    idx_t before = r * (r + 1ULL) / 2ULL;       /* number of terms before row r */
    idx_t j      = n - before - 1ULL;          /* column index (exponent of the smaller power) */

    /* compute 16^r and 16^j (use mul by 16 to avoid out‑of‑range shifts) */
    seq_t val = pow16(r) + pow16(j);
    return val;
}
