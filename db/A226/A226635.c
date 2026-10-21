/*
 * A226635  Expansion of psi(x^4) / f(-x) in powers of x where psi(), f() are
 *          Ramanujan theta functions.
 * https://oeis.org/A226635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1, 2*k*(k−1)≤n} p(n−2*k*(k−1)) where p(m) is the
 *            partition number of m (the coefficients of 1/∏_{k≥1}(1−x^k)).
 *            This implementation precomputes partition numbers up to the
 *            largest argument encountered, re‑computing them from scratch
 *            whenever the required bound grows
 * Range:     u8 n <= 15, u16 n <= 40, u32 n <= 121, u64 n <= 400,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *partitions = NULL;      /* memoised partition numbers */
static size_t partition_cap = 0;      /* current allocated size */

/* Compute partition numbers p[0..N] from scratch and store in partitions[]. */
static void compute_partitions_upto(idx_t N)
{
    if (partition_cap > (size_t)N)
        return;                  /* already have everything needed */

    size_t new_cap = (size_t)N + 1;
    seq_t *tmp = (seq_t *)realloc(partitions, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                  /* out of memory */
    partitions = tmp;

    /* initialise all entries to 0 */
    for (size_t i = 0; i < new_cap; ++i)
        partitions[i] = 0;
    partitions[0] = 1;             /* p(0) = 1 */

    /* classic additive partition DP */
    for (idx_t i = 1; i <= N; ++i) {
        for (idx_t j = i; j <= N; ++j)
            partitions[j] += partitions[j - i];
    }
    partition_cap = new_cap;
}

seq_t a(idx_t n)
{
    compute_partitions_upto(n);

    seq_t result = 0;
    for (uint64_t k = 1; ; ++k) {
        __uint128_t t = (__uint128_t)2 * k * (k - 1);
        if (t > n)
            break;
        result += partitions[(size_t)(n - (idx_t)t)];
    }
    return result;
}
