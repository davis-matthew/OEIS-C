/*
 * A116647  Triangle of number of partitions that fit in an n X n box (but not
 *          in an (n-1) X (n-1) box) with Durfee square k.
 * https://oeis.org/A116647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   T(n,k) = binom(n,k)^2 - binom(n-1,k)^2 * a(n) is the row‑major
 *            flattening of T(n,k) with offset 1. * a(1)=T(1,1)=1,
 *            a(2)=T(2,1)=3, a(3)=T(2,2)=1, …
 * Range:     u8 n <= 17, u16 n <= 59, u32 n <= 179, u64 n <= 644,
 *            u128 n <= 1275, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Return the n-th term of sequence A116647 (flattened triangle). */
seq_t a(idx_t n)
{
    if (n < 1)                     /* sequence starts at offset 1 */
        return 0;

    /* Find the row r such that the cumulative number of terms up to
       row r-1 is < n and up to row r is ≥ n. */
    idx_t r   = 1;
    idx_t cum = 1;                 /* cumulative count after row r */

    while (cum < n) {
        ++r;
        cum += r;                  /* each row adds r terms */
    }

    /* Terms before the current row and the 1‑based column k within row r */
    idx_t prev = cum - r;           /* terms before row r  */
    idx_t k    = n - prev;           /* column index in row r */

    /* Compute binomial coefficients.  binom(a,b) returns seq_t and
       handles the (a<b) case by returning 0. */
    seq_t b1 = binom(r, k);          /* C(r,k)   */
    seq_t b2 = binom(r - 1, k);      /* C(r-1,k) */

    /* Return T(r,k) = C(r,k)^2 - C(r-1,k)^2.  All arithmetic is in
       seq_t.  For arbitrary‑precision seq_t this is exact. */
    return b1 * b1 - b2 * b2;
}
