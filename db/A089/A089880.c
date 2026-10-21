/*
 * A089880  Number of cycles in range [A014137(n-1)..A014138(n-1)] of
 *          permutation A069772.
 * https://oeis.org/A089880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A000108(n)+A089849(n))/2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binom_helper, cat_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence values are expressed with the type seq_t (unsigned integer or
   arbitrary‑precision integer).  The index type is idx_t (unsigned 64‑bit). */

#include <stdint.h>
#include "../utils/binom_helper.h"
#include "../utils/cat_helper.h"

/* ------------------------------------------------------------------ */
/* Helper: binomial coefficient C(n,k) = n!/(k!(n−k)!)                 */
/* Uses the multiplicative formula to keep intermediate values exact. */

/* ------------------------------------------------------------------ */
/* Helper: Catalan number C_n = binom(2n,n)/(n+1)                       */

/* ------------------------------------------------------------------ */
/* Main function for OEIS A089880                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* A000108(n): Catalan number */
    seq_t cat_val = cat_helper(n);

    /* A089849(n):
          if n is even -> binom(n, n/2)
          if n is odd  -> Catalan((n-1)/2)  */
    seq_t a089849_val;
    if ((n & 1ULL) == 0) {               /* n is even */
        a089849_val = binom_helper(n, n / 2);
    } else {                             /* n is odd */
        a089849_val = cat_helper((n - 1) / 2);
    }

    /* a(n) = (C_n + A089849(n))/2 */
    return (cat_val + a089849_val) / (seq_t)2;
}
