/*
 * A173622  Triangle T(n,m) read by rows: The number of m-Schroeder paths of
 *          order n with 2 diagonal steps.
 * https://oeis.org/A173622
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(r,m) where
 *   r = smallest integer with r*(r-1)/2 >= n-1 ,
 *   m = (n-1) - (r-1)*(r-2)/2 ,
 *   T(r,m) = multinomial(r*m + r - 2; r*m - 2, r - 2, 2) / (r*m - 1)
 *   multinomial(x; a,b,c) = x! / (a! b! c!)
 *
 * The binomial coefficient is computed by the helper binom from
 * ../utils/binom.h.  The greatest common divisor is computed by
 * gcd from ../utils/gcd.h.  All arithmetic is performed in seq_t
 * after reducing the fraction to avoid overflow; intermediate
 * products are held in a wider type.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/gcd.h"

/* Return a(n) for n >= 2, 0 otherwise. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                     /* offset starts at 2 */

    /* Map the 1‑based sequence position to (row r, column m). */
    idx_t k = n - 1;                 /* position counting from 1 */
    idx_t r = 2;
    /* find the smallest r with r*(r-1)/2 >= k  (r <= 141 for k<10000) */
    while ((r * (r - 1)) / 2 < k)
        ++r;
    idx_t prev = (r - 1) * (r - 2) / 2;
    idx_t m = k - prev;              /* column index, 1‑based */

    /* Compute the trinomial coefficient components. */
    idx_t total = r * m + r - 2;      /* r*m + r - 2 */
    idx_t a1    = r * m - 2;          /* r*m - 2 */

    seq_t binom1 = binom(total, a1);  /* C(total, a1)   */
    seq_t binom2 = binom(r, 2);       /* C(r, 2)        */
    seq_t denom  = (seq_t)r * m - 1;  /* r*m - 1        */

    /* Reduce the fraction before multiplying to keep the numbers small. */
    seq_t g = gcd(binom2, denom);
    binom2   /= g;
    denom    /= g;

    g = gcd(binom1, denom);
    binom1   /= g;
    denom    /= g;

    /* denom should now be 1.  Use a wider type to compute the product. */
    seq_t result;
    if (sizeof(seq_t) <= 8) {
        unsigned long long prod = (unsigned long long)binom1 * (unsigned long long)binom2;
        result = (seq_t)(prod / denom);
    } else {
        unsigned __int128 prod = (unsigned __int128)binom1 * (unsigned __int128)binom2;
        result = (seq_t)(prod / denom);
    }

    return result;
}
