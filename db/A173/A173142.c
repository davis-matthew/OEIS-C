/*
 * A173142  a(n) = n^n - (n-1)^(n-1) - (n-2)^(n-2) - ... - 1.
 * https://oeis.org/A173142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - Σ_{k=1}^{n-1} k^k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif
#include "../utils/big_pow.h"

static idx_t max_n = 0;                 /* largest n that has been prefixed */

#ifdef __cplusplus
static std::vector<seq_t> prefix_vec;    /* cumulative sums, prefix_vec[i] = Σ_{k=1}^i k^k */
#else
static seq_t *prefix_ptr = NULL;        /* same as prefix_vec in the C case */
#endif

static void ensure_prefix(idx_t n)
{
    if (n <= max_n) return;             /* nothing to do */

#ifdef __cplusplus
    /* C++ path: use std::vector */
    if (prefix_vec.empty()) {
        prefix_vec.resize(1);
        prefix_vec[0] = 0;
    }
    /* grow the vector to hold up to n */
    prefix_vec.resize(n + 1);
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t base = (seq_t)i;
        seq_t val  = big_pow(base, i);
        prefix_vec[i] = prefix_vec[i - 1] + val;
    }
    max_n = n;
#else
    /* C path: manual reallocation */
    size_t new_size = (size_t)(n + 1);
    if (prefix_ptr == NULL) {
        prefix_ptr = malloc(new_size * sizeof(seq_t));
        if (prefix_ptr == NULL) abort();
        prefix_ptr[0] = 0;
        for (idx_t i = 1; i <= n; ++i) {
            seq_t base = (seq_t)i;
            seq_t val  = big_pow(base, i);
            prefix_ptr[i] = prefix_ptr[i - 1] + val;
        }
    } else {
        seq_t *tmp = realloc(prefix_ptr, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        prefix_ptr = tmp;
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t base = (seq_t)i;
            seq_t val  = big_pow(base, i);
            prefix_ptr[i] = prefix_ptr[i - 1] + val;
        }
    }
    max_n = n;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* value outside the offset is unspecified */

    ensure_prefix(n);                    /* guarantees prefix[n-1] exists */

    /* Compute n^n and subtract the sum of previous terms */
    seq_t big = big_pow((seq_t)n, n);
#ifdef __cplusplus
    seq_t sumprev = prefix_vec[n - 1];
#else
    seq_t sumprev = prefix_ptr[n - 1];
#endif
    return big - sumprev;
}
