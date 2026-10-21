/*
 * A075779  Triangle T(n,k) = f(n,k,n-1), n >= 2, 1 <= k <= n-1, where f is
 *          given below.
 * https://oeis.org/A075779
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The sequence is the flattened entries of the triangle defined by
 *            T(n,k)=f(n,k,n-1) where
 *            f(n,p,k)=binomial(n,k)*hypergeom([1-k,-p,p-n],[1-n,1],1). For
 *            OEIS A075779, a(2)=2 and subsequent terms are listed by reading
 *            the triangle rows from n=2 upward, keeping the offset 2. This
 *            implementation evalu...
 * Range:     u8 n <= 12, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"

static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t bj    = binom(k, j);
        seq_t bjk   = binom(n - k, j);
        seq_t term  = bj * bjk;
        term = term * (n - 1 - j);
        sum += term;
    }
    // multiply by n, divide by (n-1); integer division is exact
    seq_t t = sum * n;
    t = t / (n - 1);
    return t;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          // undefined for offsets < 2

    /* Convert the OEIS index to a 0‑based position within the
       flattened triangle.  Index n=2 corresponds to position 0.  */
    uint64_t idx0 = (uint64_t)n - 2ULL;

    /* Find the row r such that r(r-1)/2 > idx0 and (r-1)(r-2)/2 ≤ idx0.
       Use a double square root to get an initial guess and correct it. */
    long double root = sqrtl((long double)1.0 + 8.0L * idx0);
    uint64_t r = (uint64_t)((1.0L + root) / 2.0L) + 1; /* first guess */

    /* Correct possible off‑by‑one errors caused by rounding. */
    while ((unsigned __int128)(r - 1) * (r - 2) / 2 > idx0) --r;
    while ((unsigned __int128)r * (r - 1) / 2 <= idx0) ++r;

    /* Column within the row (1‑based).  */
    uint64_t pre = (unsigned __int128)(r - 2) * (r - 1) / 2;
    uint64_t col = idx0 - pre + 1;   /* 1‑based column index   */

    /* Compute the triangle entry.  */
    return compute_T(r, col);
}
