/*
 * A094496  Triangle read by rows: T(n,k) = binomial(n,k) - binomial(n,k) mod
 *          n^2, with T(0,0) = 1.
 * https://oeis.org/A094496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl, less
 * Formula:   a(n) is the element at index n of the triangle * T(r,k) =
 *            binomial(r,k) - binomial(r,k) mod r^2, * read by rows. r and k
 *            are obtained from the * triangular numbering of the sequence
 * Range:     u8 n <= 39, u16 n <= 196, u32 n <= 645, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case: the very first entry of the triangle */
    if (n == 0U)
        return (seq_t)1;          /* T(0,0) = 1 */

    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2
       (triangular numbering, 0-based).  Linear search is fine
       because for n up to 10 000 the row index is at most ~141. */
    idx_t r = 0U;
    for (;; ++r) {
        idx_t tri = (r * (r + 1U)) / 2U;
        if (tri > n) {
            r -= 1U;            /* step back to the last valid row */
            break;
        }
    }

    /* Column index within the identified row */
    idx_t tri = (r * (r + 1U)) / 2U;
    idx_t k   = n - tri;

    /* For r = 0 we already returned T(0,0) = 1, but
       the following works for r > 0 as well. */

    /* Compute the binomial coefficient binomial(r,k) */
    seq_t bin = binomial(r, k);

    /* Determine if r^2 fits into seq_t.
       If it does not, T(r,k) = 0 because (r^2 > max_seq_t >= bin). */
    size_t bits = sizeof(seq_t) * 8;
    idx_t   max_seq_t = bits >= 64 ? (idx_t)-1 : ((idx_t)1 << bits) - 1;
    unsigned long long r_sq_ull = (unsigned long long)r * r;

    if (r_sq_ull > (unsigned long long)max_seq_t)
        return (seq_t)0;

    /* r^2 now fits into seq_t, so we can use seq_t arithmetic. */
    seq_t r_sq = (seq_t)r;
    r_sq = r_sq * r_sq;          /* r^2 in seq_t */

    seq_t mod = bin % r_sq;      /* binomial mod r^2 */
    seq_t res = bin - mod;       /* binomial – (binomial mod r^2) */

    return res;
}
