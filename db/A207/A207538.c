/*
 * A207538  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A207537; see Formula section.
 * https://oeis.org/A207538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binom, cum_count, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207538:  Triangle of coefficients of v(n,x).
   The flattening order is: for each n=1,2,3,... list the
   coefficients of v(n,x) from the constant term upward.
   With r = row index (0‑based, r = n-1) and k = column index
   (0‑based within the row), the value is
        a(n) = C(r-k, k) * 2^(r - 2k)
   where C = binomial coefficient.  The mapping (r,k) from n is obtained
   by accumulating the row lengths L(r)=floor(r/2)+1.  The cumulative
   counts satisfy
        Cum(r) = (p+1)^2            if r = 2p,
        Cum(r) = (p+1)(p+2)         if r = 2p+1.
   The code finds r by a simple loop (r is <~ 2*sqrt(n)) and then
   computes k, the binomial coefficient, and the power of two.  The
   helper functions binom and pow2 from the utils library are
   employed for exact arithmetic. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/cum_count.h"

/* Main sequence function.  The index n is 1‑based: a(1) is the first term. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */

    /* Find row r such that cum_count(r) >= n and cum_count(r-1) < n. */
    idx_t r = 0;
    while (cum_count(r) < n) r++;

    /* Previous cumulative count (before row r) */
    idx_t prev = (r > 0) ? cum_count(r - 1) : 0;

    /* Column index within row r (0‑based) */
    idx_t k = n - prev - 1;

    /* Compute binomial coefficient C(r-k, k) and 2^(r-2k). */
    seq_t bin   = binom(r - k, k);
    seq_t pow2ex = pow2(r - 2 * k);

    return bin * pow2ex;
}
