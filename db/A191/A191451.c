/*
 * A191451  Dispersion of (3*n-2), for n>=2, by antidiagonals.
 * https://oeis.org/A191451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   t_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dispersion of (3*n-2), for n≥2, by antidiagonals.
   The dispersion array D has rows
        (t(r), s(t(r)), s(s(t(r))), …)
   where s(k) = 3*k+1 (k≥1) and t is the complement of s
   (t(1)=1).  a(n) is the n‑th entry read by antidiagonals. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/t_value.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n<1, but harmless */

    /* Find diagonal number d such that (d-1)*d/2 ≥ n. */
    idx_t d = 2;
    while ((unsigned __int128)(d - 1) * d / 2 < n)
        ++d;

    /* Number of entries before this diagonal. */
    idx_t before = (unsigned __int128)(d - 2) * (d - 1) / 2;
    /* Zero‑based position inside the diagonal. */
    idx_t pos = n - before - 1;
    /* Row and column indices (1‑based). */
    idx_t r = pos + 1;
    idx_t c = d - r;

    /* Start with t(r). */
    seq_t v = t_value(r);

    /* Apply s(k)=3*k+1 (c‑1) times. */
    for (idx_t m = 1; m < c; ++m)
        v = v * 3 + 1;

    return v;
}
