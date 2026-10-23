/*
 * A137525  Triangle read by rows: T(n, k) = n! * [t^n*x^k] (1-1/t^2) *
 *          exp(t*x), 0 <= k <= n + 2.
 * https://oeis.org/A137525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  less, tabf, sign
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 38, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A137525: Triangle read by rows
 * a(n) is obtained by flattening the triangle
 *   T(n,k) = n! * [t^n x^k] (1-1/t^2) * exp(t*x), 0 ≤ k ≤ n+2
 *   T(n,n)   = (n+2)!,  T(n,n+2) = -n!,  all other entries are 0.
 * The sequence a(n) is the row‑by‑row concatenation of T.
 * For an index n, let r be the unique row with
 *   (r+1)(r+6)/2 > n,
 * let pos = n - r(r+5)/2 (position inside row r).
 * Then
 *   a(n) = (r+2)!          if pos == r
 *   a(n) = -(r)!           if pos == r+2
 *   a(n) = 0               otherwise
 *
 * This implementation uses the helper
 *   seq_t factorial(idx_t n)
 * provided in the build utilities.
 */

#include "../utils/factorial.h"

/* Find the row r that contains the flat index n. */
static idx_t row_of(idx_t n)
{
    idx_t r = 0;
    /* cumulative number of elements up to and including row r */
    while ( ((r + 1) * (r + 6)) / 2 <= n )
        ++r;
    return r;
}

/* Return the sequence value a(n). */
seq_t a(idx_t n)
{
    idx_t r = row_of(n);
    /* number of elements before row r: r(r+5)/2 */
    idx_t prev = (r * (r + 5)) / 2;
    idx_t pos  = n - prev;                /* position in row r */

    if (pos == r)            /* main diagonal: (r+2)! */
        return factorial(r + 2);
    else if (pos == r + 2)   /* anti‑diagonal: -(r)! */
        return -(seq_t)factorial(r);
    else
        return 0;
}
