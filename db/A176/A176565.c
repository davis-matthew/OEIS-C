/*
 * A176565  Triangle read by rows: T(n,k) = binomial(P(n)+k,k) +
 *          binomial(P(n)+n-k,n-k) - binomial(P(n)+n,n) where P(n) =
 *          A000041(n) is the number of partitions of n.
 * https://oeis.org/A176565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   big_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A176565
 * Triangle read by rows: T(n,k) = binomial(P(n)+k,k) + binomial(P(n)+n-k,n-k) - binomial(P(n)+n,n)
 * where P(n) = A000041(n) is the number of partitions of n.
 * a(0) is the first term.  The sequence is read by rows, so a(n) is
 * the element at index n in the flattened triangle. */

#include <stdlib.h>
#include "../utils/big_binom.h"

/* ------------------------------------------- */
/*  Helper: multiplicative binomial coefficient */
/*  Computes binomial(a, b) with a as seq_t, b <= a   */

/* ------------------------------------------- */
/*  Partition numbers cache up to the needed n */
static seq_t *partition_cache = NULL;
static idx_t partition_cache_len = 0;

/*  Fill partition_cache[0..n] using Euler's pentagonal theorem.  */
static void ensure_partitions(idx_t n)
{
    if (partition_cache_len > n)
        return;

    /* Allocate array large enough to hold up to n (inclusive).  */
    idx_t need = n + 1;
    seq_t *tmp = (seq_t *)realloc(partition_cache, (size_t)need * sizeof(seq_t));
    if (!tmp)
        exit(1);                 /* memory exhausted */
    partition_cache = tmp;

    /* Compute missing partition numbers.  */
    for (idx_t i = partition_cache_len; i < need; ++i) {
        if (i == 0) {
            partition_cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i)
                break;
            int sign = (k & 1) ? 1 : -1;
            sum += (sign == 1) ? partition_cache[i - pent1]
                               : -partition_cache[i - pent1];

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i)
                sum += (sign == 1) ? partition_cache[i - pent2]
                                   : -partition_cache[i - pent2];
        }
        partition_cache[i] = sum;
    }
    partition_cache_len = need;
}

/* ------------------------------------------- */
/*  Sequence function a(idx_t n)                 */
seq_t a(idx_t n)
{
    /* Locate row r and column k within that row:
       row r has r+1 elements, and the total number of elements before row r is r(r+1)/2.  */
    idx_t r = 0;
    while ((((unsigned __int128)(r + 1) * (r + 2)) / 2) <= n)
        ++r;

    idx_t before = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t k = n - before;

    /* Ensure partition number p(r) is available.  */
    ensure_partitions(r);
    seq_t P = partition_cache[r];

    /* Compute the three binomial terms.  */
    seq_t term1 = big_binom(P + (seq_t)k, k);
    seq_t term2 = big_binom(P + (seq_t)(r - k), r - k);
    seq_t term3 = big_binom(P + (seq_t)r, r);

    return term1 + term2 - term3;
}
