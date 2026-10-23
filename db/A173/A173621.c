/*
 * A173621  Triangle of Generalized Runyon numbers R_{n,k}^(4) read by rows.
 * https://oeis.org/A173621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(r,c)*binomial(4*r,c-1)/r where r is the least
 *            integer with r(r+1)/2 ≥ n and c = n - (r-1)r/2 (1 ≤ c ≤ r)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code uses the utility function binom(idx_t a, idx_t b)
   which returns the binomial coefficient as seq_t. */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Compute r and c from n.  For the sizes involved (n≤10 000)
       a simple linear search is fast enough. */
    idx_t r = 1;
    while (n > r * (r + 1) / 2)
        r++;
    idx_t c = n - (r - 1) * r / 2;   /* 1‑based column index */

    seq_t bin1 = binom(r, c);           /* binom(r,c) */
    seq_t bin2 = binom(4 * r, c - 1);   /* binom(4r,c-1) */
    seq_t prod = bin1 * bin2;
    seq_t res  = prod / (seq_t)r;       /* exact division */

    return res;
}
