/*
 * A152883  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} in which k is an excedance (n >= 2, 1 <= k <= n-1). An
 *          excedance of a permutation p is a value j such that p(j) > j.
 * https://oeis.org/A152883
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: a(n) lists the entries of the triangle
   defined by T(r,k) = (r-1)! * (r-k) for r ≥ 2, 1 ≤ k ≤ r-1.
   For a given index n ≥ 2, let r be the smallest integer such that
   (r-1)r/2 ≥ n-1.  Then k = n-1 - (r-2)(r-1)/2 and
   a(n) = T(r,k) = (r-1)! * (r-k).  */

#include <math.h>
#include <stdlib.h>

/* Static cache for factorials: fact[i] = i! (0! = 1) */
static seq_t *fact = NULL;
static size_t fact_len = 0;

/* Ensure factorials up to value 'max_i' are computed. */
static void ensure_factorial(idx_t max_i)
{
    /* max_i is 0‑based; we need array up to index max_i inclusive */
    size_t needed = (size_t)max_i + 1;
    if (fact_len < needed) {
        size_t new_cap = fact_len ? fact_len * 2 : 2;
        if (new_cap < needed) new_cap = needed;
        fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        /* If array wasn't previously allocated, initialize first entry */
        if (fact_len == 0) fact[0] = 1;
        /* Compute missing factorials */
        for (size_t i = fact_len; i < new_cap; ++i) {
            if (i == 0)
                fact[i] = 1;
            else
                fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_len = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* out of range, value is undefined */

    /* n-1 is the 1‑based position within the flattened triangle */
    idx_t m = n - 1;

    /* Compute r: smallest integer with (r-1)*r/2 >= m */
    double val = (1.0 + sqrt(1.0 + 8.0 * (double)m)) / 2.0;
    idx_t r = (idx_t)ceil(val);

    /* Adjust r in case of rounding errors (for safety) */
    while (r * (r - 1) / 2 < m) r++;
    while ((r - 1) * (r - 2) / 2 >= m) r--;

    /* Number of terms before row r */
    idx_t before = (r - 2) * (r - 1) / 2;
    /* Position within the row (1‑based) */
    idx_t k = m - before;          /* k ranges from 1 to r-1 */

    /* Ensure factorial up to (r-1) is available */
    ensure_factorial(r - 1);
    seq_t fac = fact[r - 1];        /* (r-1)! */

    return fac * (seq_t)(r - k);    /* (r-1)! * (r-k) */
}
