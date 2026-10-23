/*
 * A182971  Triangle read by rows: coefficients in expansion of Q(n) =
 *          (x-n^2)*(x-(n-2)^2)*(x-(n-4)^2)*...*(x-(1 or 2)^2), highest powers
 *          first.
 * https://oeis.org/A182971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 26, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the coefficient of x^{d - p} in
 * Q(r) = ∏(x – k²) where the k’s are the even integers up to r (if r is even)
 * or the odd integers up to r (if r is odd).  
 * The index n is first decomposed into the row r and position p within that row
 * in the flattened triangle (row 0 contains 1 term, row 1 contains 2 terms,
 * row 2 contains 2 terms, row 3 contains 3 terms, …).  The required
 * coefficient equals (-1)^p · eₚ, where eₙ is the elementary symmetric
 * sum of degree p of the numbers {k²}.
 */

#include <stdlib.h>   /* for calloc/ free */

seq_t a(idx_t n)
{
    /* Determine the row r and position p (0‑based) within the row */
    idx_t sum = 0;          /* cumulative number of terms up to the previous row */
    idx_t r   = 0;
    for (;;)
    {
        idx_t len = (r % 2 == 0) ? (r / 2 + 1) : ((r + 3) / 2);
        if (sum + len > n) break;
        sum += len;
        ++r;
    }
    idx_t p = n - sum;           /* position within the row */

    /* Leading coefficient is always 1 */
    if (p == 0) return 1;

    /* Number of factors in Q(r) */
    idx_t d = (r % 2 == 0) ? (r / 2) : ((r + 1) / 2);

    /* Allocate array for elementary symmetric sums up to order p */
    size_t const sz = (size_t)p + 1;
    seq_t *e = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!e) return 0;            /* allocation failure – unlikely for small n */
    e[0] = 1;                    /* e₀ = 1 */

    /* Process each factor a_i = k², updating e from high degree downwards */
    size_t cur = 0;             /* number of factors processed so far */
    if (r % 2 == 0)             /* even r – factors are 2,4,…,r */
    {
        for (idx_t i = 1; i <= d; ++i)
        {
            idx_t k = 2 * i;
            seq_t a_val = (seq_t)k * (seq_t)k;
            ++cur;
            size_t limit = cur < p ? cur : (size_t)p;
            for (size_t s = limit; s > 0; --s)
                e[s] += e[s - 1] * a_val;
        }
    }
    else                         /* odd r – factors are 1,3,…,r */
    {
        for (idx_t i = 0; i < d; ++i)
        {
            idx_t k = 2 * i + 1;
            seq_t a_val = (seq_t)k * (seq_t)k;
            ++cur;
            size_t limit = cur < p ? cur : (size_t)p;
            for (size_t s = limit; s > 0; --s)
                e[s] += e[s - 1] * a_val;
        }
    }

    seq_t result = e[p];
    if (p % 2 == 1) result = -result;   /* sign alternates with position p */
    free(e);
    return result;
}
