/*
 * A194450  Vertex number of a rectangular spiral which contains exactly
 *          between its edges the successive sections of the modular table of
 *          partitions of the positive integers.
 * https://oeis.org/A194450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(2n)   = P(n) + n*(n+1)/2
 *          a(2n‑1) = P(n) + n*(n+1)/2 – n
 *
 * where P(n) = sum_{k=1..n} p(k) and p(k) is the partition number of k.
 * P(n) is computed once by a classic coin‑change DP and stored in a
 * static table.  The function works for any seq_t/idx_t supplied by the
 * build and is valid in both C11 and C++17.
 */

#include <stdbool.h>
#include <stddef.h>

/* maximum m needed for n up to 10000 (ceil(10000/2) = 5000) */
#define MAX_M 5000U

/* static tables for partitions and their prefix sums */
static seq_t  partitions[MAX_M + 1];
static seq_t  prefix_sums[MAX_M + 1];
static bool   tables_computed = false;

/* Compute partition numbers up to max_m and their prefix sums. */
static void compute_tables(size_t max_m)
{
    /* initialise */
    partitions[0] = (seq_t)1;
    for (size_t i = 1; i <= max_m; ++i)
        partitions[i] = (seq_t)0;

    /* coin‑change DP to generate partition numbers */
    for (size_t coin = 1; coin <= max_m; ++coin) {
        for (size_t j = coin; j <= max_m; ++j) {
            partitions[j] += partitions[j - coin];
        }
    }

    /* prefix sums of partition numbers */
    prefix_sums[0] = (seq_t)0;
    for (size_t i = 1; i <= max_m; ++i) {
        prefix_sums[i] = prefix_sums[i - 1] + partitions[i];
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Ensure tables are ready; 5000 is sufficient for n ≤ 10000 */
    if (!tables_computed) {
        compute_tables((size_t)MAX_M);
        tables_computed = true;
    }

    /* m = ceil(n/2) */
    size_t m = (size_t)((n + (idx_t)1) / (idx_t)2);

    /* triangular number T(m) = m*(m+1)/2 */
    seq_t tri = ((seq_t)m) * ((seq_t)(m + (idx_t)1)) / (seq_t)2;

    /* result = P(m) + T(m)  minus m if n is odd */
    seq_t res = prefix_sums[m] + tri;
    if (n & (idx_t)1)        /* odd n: subtract m */
        res -= (seq_t)m;

    return res;
}
