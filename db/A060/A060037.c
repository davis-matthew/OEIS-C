/*
 * A060037  Triangular array T read by rows: T(n,k)=k^2 mod n, for
 *          k=1,2,...,[n/2], n=2,3,...
 * https://oeis.org/A060037
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy, nice
 * Formula:   a(n) = k^2 mod r, where r is the smallest integer ≥2 such that *
 *            Σ_{i=2}^{r} ⌊i/2⌋ ≥ n−1, and k = (n−1) − Σ_{i=2}^{r−1} ⌊i/2⌋
 * Range:     u8 n <= 2501, u16 n <= 2501, u32 n <= 2501, u64 n <= 2501,
 *            u128 n <= 2501, big any n
 * Helpers:   cumulative, square_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (row r of the triangular array T(n,k)=k^2 mod n, read row‑by‑row). */

#include <stdint.h>
#include <math.h>
#include "../utils/cumulative.h"
#include "../utils/square_mod.h"

seq_t a(idx_t n)
{
    /* n ≥ 2, the OEIS offset is 2 so a(2) is the first term. */
    idx_t target = n - 1;                /* we search for r with S(r) ≥ target */

    /* Approximate r by solving r^2/4 ≈ target => r ≈ 2*sqrt(target) */
    idx_t M = (idx_t)sqrtl((long double)target);

    idx_t r;
    if (cumulative(2 * M) >= target)
        r = 2 * M;
    else if (cumulative(2 * M + 1) >= target)
        r = 2 * M + 1;
    else
        r = 2 * M + 2;   /* guarantees the bound */

    idx_t prevCum = cumulative(r - 1);
    idx_t k = target - prevCum;          /* 1‑based index within the row */

    return square_mod(r, k);
}
