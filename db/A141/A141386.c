/*
 * A141386  Conjectured to be complete list of numbers not of the form x^2 +
 *          y^2 + 5*triangular number.
 * https://oeis.org/A141386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th number NOT representable as x^2 + y^2 + 5*T_k,
 *            where T_k = k(k+1)/2
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented by generating all sums of two squares up to a fixed
   limit and testing each natural number for representability.   */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static const idx_t MAX_LIMIT = 20000000;          /* 20 million */
static bool *sum_sq = NULL;                       /* sum‑of‑two‑squares table */
static idx_t sum_sq_limit = 0;                    /* current table size */

/* Precompute all values <= limit that are expressible as a sum of two squares. */
static void init_sums_of_two_squares(idx_t limit)
{
    if (sum_sq != NULL) return;        /* already initialized */

    sum_sq_limit = limit;
    sum_sq = (bool *)malloc((sum_sq_limit + 1) * sizeof(bool));
    if (!sum_sq) abort();

    for (idx_t x = 0; x * x <= sum_sq_limit; ++x) {
        idx_t xx = x * x;
        for (idx_t y = 0; xx + y * y <= sum_sq_limit; ++y) {
            sum_sq[xx + y * y] = true;
        }
    }
}

/* Check if integer n (up to sum_sq_limit) is representable as
   a^2 + b^2 + 5*T_k. */
static bool is_representable(idx_t n)
{
    /* k = 0 -> term 5*T_k = 0, check sum of two squares directly */
    if (sum_sq[n]) return true;

    /* try all k >= 1 while 5*T_k <= n */
    for (idx_t k = 1;; ++k) {
        /* term5 = 5 * k * (k+1) / 2 */
        idx_t term5 = 5 * k * (k + 1) / 2;
        if (term5 > n) break;
        idx_t r = n - term5;
        if (r <= sum_sq_limit && sum_sq[r]) return true;
    }
    return false;
}

/* Returns the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */

    init_sums_of_two_squares(MAX_LIMIT);

    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        if (!is_representable(m)) {
            ++count;
            if (count == n) return (seq_t)m;
        }
        if (m == sum_sq_limit) {          /* safety: should not happen for n<=10000 */
            /* If we exceed the precomputed limit, extend the table and continue */
            size_t new_limit = sum_sq_limit * 2;
            if (new_limit < m + 1000) new_limit = m + 1000;
            init_sums_of_two_squares(new_limit);
        }
    }
}
