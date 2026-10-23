/*
 * A126179  Triangle read by rows: T(n,k) is number of hex trees with n edges
 *          and k branches (1 <= k <= n).
 * https://oeis.org/A126179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row containing n and k = n -
 *            triangular(r-1). The triangle entries are computed by
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   binomial, catalan, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   T(r,k) = 3^(r-k+1) * binom(r-1,k-1) * cat((k-1)/2)  if k is odd
      T(r,k) = 3^(r-k)   * binom(r-1,k-1) * cat(k/2)        if k is even
   (catalan = c(i) = binom(2i,i)/(i+1) ) */

#include "../utils/pow3.h"
#include "../utils/binomial.h"
#include "../utils/catalan.h"

/* Compute the n-th triangular number as an idx_t (safe for n up to ~10⁵) */
static inline idx_t triangular_idx(idx_t r)
{
    return r * (r + 1) / 2;
}

/* Return the a(n) of sequence A126179.  The index n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, return 0 */

    /* Find row r with triangular(r-1) < n <= triangular(r) */
    idx_t r = 1;
    while (triangular_idx(r) < n) ++r;

    idx_t prev_tri = triangular_idx(r - 1);
    idx_t k = n - prev_tri;               /* 1 <= k <= r */

    /* Compute T(r,k) using the closed form */
    seq_t comb = binomial(r - 1, k - 1);   /* binomial coefficient */
    seq_t cat;
    idx_t exp;

    if ((k & 1) == 0) {                  /* k even */
        exp = r - k;
        cat = catalan(k / 2);
    } else {                              /* k odd */
        exp = r - k + 1;
        cat = catalan((k - 1) / 2);
    }

    seq_t p3 = pow3(exp);                 /* 3^exp */
    return p3 * comb * cat;               /* exact arithmetic in seq_t */
}
