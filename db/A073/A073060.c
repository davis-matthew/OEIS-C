/*
 * A073060  Multiplication table for 1 and odd primes, read by antidiagonals.
 * https://oeis.org/A073060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = p(i) * p(j) where s = ⌊(√(8⋅n+1) − 1)/2⌋, i = n −
 *            s⋅(s+1)/2, j = s − i, p(0) = 1, p(k) (k≥1) = k‑th odd prime
 *            (3,5,7,…)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   get_odd_prime_at, sqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers ----------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_odd_prime_at.h"
#include "../utils/sqrt128.h"

/*  Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    /*  Compute antidiagonal index s using integer arithmetic  */
    unsigned __int128 X = ((unsigned __int128)8 * n) + 1;
    unsigned __int128 t = sqrt128(X);              /* t = ⌊√X⌋          */
    idx_t s = (idx_t)((t - 1) >> 1);               /* s = ⌊(t−1)/2⌋     */

    /*  Start of the s‑th diagonal  */
    unsigned __int128 diag_start = ((unsigned __int128)s * (s + 1)) >> 1; /* s*(s+1)/2 */

    idx_t i = n - (idx_t)diag_start;             /* row index   (0 ≤ i ≤ s) */
    idx_t j = s - i;                              /* col index   (0 ≤ j ≤ s) */

    /*  Fetch primes (or 1 for index 0) and return their product  */
    seq_t left  = get_odd_prime_at(i);
    seq_t right = get_odd_prime_at(j);
    return left * right;
}
