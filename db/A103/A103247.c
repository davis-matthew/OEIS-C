/*
 * A103247  Triangle read by rows: T(n,k) is the coefficient of x^k (0<=k<=n)
 *          in the monic characteristic polynomial of the n X n matrix with
 *          3's on the diagonal and 1's elsewhere (n>=1). Row 0 consists of
 *          the single term 1.
 * https://oeis.org/A103247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coefficient of x^k in (x‑2)^(r‑1)·(x‑ (r+2)), where r =
 *            row_of(n) (the unique r with r(r+1)/2 ≤ n < (r+1)(r+2)/2) and k
 *            = n – r(r+1)/2 (column index in the r‑th row of the triangle)
 * Range:     i8 n <= 15, i16 n <= 56, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binom, pow_neg2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow_neg2.h"

/* Return row index r for flat index n (0‑based).  */
static inline idx_t
row_of (idx_t n)
{
  idx_t r = 0;
  while ((r + 1) * (r + 2) / 2 <= n)
    ++r;
  return r;
}

seq_t a (idx_t n)
{
  if (n == 0)              /* first row: [1] */
    return (seq_t)1;

  idx_t r = row_of (n);         /* row index (0‑based)   */
  idx_t k = n - r * (r + 1) / 2;  /* column index in row   */

  /* Edge cases: last coefficient of every row is 1.  */
  if (k == r)
    return (seq_t)1;

  /* For k == 0 the formula simplifies: T(r,0) = -(r+2)*(-2)^{r-1}.  */
  if (k == 0)
    return -(seq_t)(r + 2) * pow_neg2 (r - 1);

  /* General case 1 ≤ k ≤ r‑1.  */
  seq_t part1 = 0, part2 = 0;

  /* part1 = binom(r‑1, k‑1) * (-2)^{r‑k} */
  {
    seq_t bin = binom (r - 1, k - 1);
    part1 = bin * pow_neg2 (r - k);
  }

  /* part2 = (r+2) * binom(r‑1, k) * (-2)^{r‑1‑k} */
  {
    seq_t bin = binom (r - 1, k);
    part2 = bin * pow_neg2 (r - 1 - k);
    part2 *= (seq_t)(r + 2);
  }

  return part1 - part2;
}
