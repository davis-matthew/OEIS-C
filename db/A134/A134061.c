/*
 * A134061  Triangle read by rows: T(n,k) = A124928(n,k) + A134059(n,k) -
 *          A007318(n,k).
 * https://oeis.org/A134061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   * A134061 is the triangle read by rows where T(n,k)=5·C(n,k) for
 *            k>0 * and the leftmost entry of row n is 1 if n==0 else 3. * The
 *            flat sequence a(n) is the concatenation of the rows
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom_mult
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/binom_mult.h"

/* Compute binomial coefficient C(n,k) as seq_t using multiplicative formula.
 * This implementation works for arbitrary‑precision seq_t as well as for
 * built‑in unsigned integers up to 64 bits. */

seq_t a(idx_t n)
{
    /* Handle the very first element separately.  */
    if (n == 0) return (seq_t)1;

    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    /* Solve  r ≈ floor((sqrt(8*n+1)-1)/2) and adjust. */
    long double d = sqrt((long double)8 * n + 1);
    idx_t r = (idx_t)((d - 1) / 2);

    /* Correct potential off‑by‑one due to floating‑point rounding. */
    while (r * (r + 1) / 2 > n) {
        --r;
    }
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column index within the row. */
    idx_t prev_terms = r * (r + 1) / 2;
    idx_t c = n - prev_terms;               /* 0 ≤ c ≤ r */

    /* Produce the value according to the definition. */
    if (c == 0)               /* leftmost entry of a nonzero row */
        return (seq_t)3;

    seq_t bin = binom_mult(r, c);
    return (seq_t)5 * bin;
}
