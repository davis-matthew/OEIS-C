/*
 * A173389  A shifted symmetrical triangular sequence:t(n,m)=If[Mod[n, 2] ==
 *          0, Binomial[n, m], Binomial[n - 1, m - 1] + If[(n - 3)*(m - 2) >=
 *          1, Binomial[n - 3, m - 2], 0]].
 * https://oeis.org/A173389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) is the entry of the shifted symmetrical triangle * t(r,c)
 *            listed row‑wise: * t(r,c) = Binomial(r,c) if r is even * =
 *            Binomial(r-1,c-1) + Binomial(r-3,c-2) if r is odd * (the second
 *            term is included only when r ≥ 4 and c ≥ 3). * The pair (r,c) is
 *            obtained by writing n = r(r+1)/2 + c * with 0...
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }

    /* Column index within the row. */
    idx_t c = n - r * (r + 1) / 2;

    if ((r & 1) == 0) {                 /* even row */
        return binomial(r, c);
    } else {                            /* odd row */
        seq_t val = 0;
        if (c > 0) {                    /* Binomial(r-1,c-1) only if c>0 */
            val = binomial(r - 1, c - 1);
        }
        if (r >= 4 && c >= 3) {         /* Optional second term */
            val += binomial(r - 3, c - 2);
        }
        return val;
    }
}
