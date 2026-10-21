/*
 * A232055  Triangular array read by rows: T(n,k) is the number of forests of
 *          rooted labeled trees such that the vertex labeled with 1 is in a
 *          component (rooted tree) of size k, n>=1, 1<=k<=n.
 * https://oeis.org/A232055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the row‑wise reading of the triangle T(n,k) =
 *            binomial(n‑1,k‑1) * k^(k‑1) * (n‑k+1)^(n‑k‑1)
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Return the n‑th term of OEIS sequence A232055 (1‑based index).  */
seq_t a(idx_t n)
{
    /* Offset starts at 1; behaviour for n == 0 is undefined, so return 0. */
    if (n == 0)
        return 0;

    /* Find the row r such that sum_{i=1}^{r} i >= n.  Use accumulation
       to avoid overflow even for very large n. */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* sum_before is the number of terms before row r. */
    idx_t sum_before = sum - r;   /* sum of 1..(r-1) */
    idx_t k = n - sum_before;     /* 1‑based column index */

    /* Compute binomial coefficient binomial(r-1, k-1).  */
    seq_t result = binomial(r - 1, k - 1);

    /* Compute k^(k-1). */
    result *= big_pow((seq_t)k, (seq_t)(k - 1));

    /* Compute (r-k+1)^(r-k-1) when the exponent is non‑negative. */
    if (r > k) {
        idx_t exp = r - k - 1;
        seq_t base = (seq_t)(r - k + 1);
        result *= big_pow(base, (seq_t)exp);
    }
    /* For r == k the factor is omitted (equals 1). */

    return result;
}
