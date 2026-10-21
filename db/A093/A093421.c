/*
 * A093421  Triangle read by rows: T(n,k) is the denominator of f(n, k) =
 *          (Product_{i = 0..k-1} (n-i))/(Sum_{i = 1..k} i) for 1 <= k <= n.
 * https://oeis.org/A093421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl, less
 * Formula:   a(n) is the denominator of the fraction f(r,k) =
 *            (r*(r-1)*…*(r-k+1)) / (k*(k+1)/2) (1 ≤ k ≤ r), where r and k are
 *            such that n is the index of the pair (r,k) when the triangle is
 *            read by rows: row r contains the k‑values 1,…,r. For example n :
 *            1 2 3 4 5 6 7 … r,k : (1,1), (2,1),(2,2),(3,1),(3,2),(...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm on 64‑bit integers – used only for gcd of the
   numerator terms with the denominator, both of which fit into 64 bits
   (the denominator is at most k(k+1)/2 ≤ 50 005 000 for k ≤ 10 000). */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Find the row 'r' that contains index n.
       Triangular number T(r) = r*(r+1)/2.  The first row is r = 1. */
    idx_t r = 1;
    idx_t tri = 1;                 /* T(1) */
    while (tri < n) {
        ++r;
        tri += r;                  /* T(r) = previous + r */
    }

    /* Column k (1‑based) within this row.  The first index of row r
       is T(r-1) + 1 = tri - r + 1. */
    idx_t start_of_row = tri - r + 1;
    idx_t k = n - start_of_row + 1;    /* 1 ≤ k ≤ r */

    /* Initial denominator = k*(k+1)/2, fits in 64 bits. */
    uint64_t denom = (uint64_t)k * (uint64_t)(k + 1) / 2;

    /* Reduce by common factors with each term of the numerator
       P(r,k) = r * (r-1) * … * (r-k+1).  Since denom is small we
       factor it out successively; after the loop):
       denom = denom / gcd(P(r,k), denom) = desired denominator. */
    for (idx_t i = 0; i < k; ++i) {
        uint64_t term = (uint64_t)r - i;     /* term = r - i  (1 ≤ i ≤ k) */
        uint64_t g = gcd_u64(term, denom);
        denom /= g;
        if (denom == 1)                /* fully reduced */
            break;
    }

    return (seq_t)denom;
}
