/*
 * A153216  A triangular sequence of powers ( suppressed powers) :
 *          t(n,m)=m^Sum[Floor[n/m^k], {k, 1, Infinity}].
 * https://oeis.org/A153216
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) term of the triangular array t(n,m) where
 *            t(n,m) = m^sum_{k=1..∞} floor(n/m^k) with n≥m≥2. The terms are
 *            listed row‑by‑row: for n=2: {2}, n=3: {2,3}, n=4: {8,3,4}, …
 *            (see OEIS A153216)
 * Range:     u8 n <= 37, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                   /* value for the offset is not defined */
    idx_t pos = n - 1;                     /* 1‑based position within the entire sequence */
    idx_t r = 2;                           /* current row number */
    /* find smallest r such that (r-1)*r/2 >= pos */
    while ((r - 1) * r / 2 < pos) r++;

    idx_t cprev = (r - 2) * (r - 1) / 2;   /* total terms before row r */
    idx_t offset = pos - cprev;            /* 1‑based offset within row r */
    idx_t m = offset + 1;                  /* column index (m ranges 2..r) */

    /* compute exponent e = Σ floor(r / m^k) */
    idx_t exp = 0;
    idx_t pw = m;
    while (pw <= r && pw > 0) {            /* pw>0 protects against overflow, though not needed here */
        exp += r / pw;
        /* next power: avoid overflow */
        if (pw > r / m) break;
        pw *= m;
    }

    return pow_int((seq_t)m, exp);          /* m^exp as seq_t */
}
