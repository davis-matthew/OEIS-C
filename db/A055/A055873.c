/*
 * A055873  a(0) = 1; a(n) = sum_{k=1 to d(n)} [a(n-k)] where d(n) is number
 *          of positive divisors of n.
 * https://oeis.org/A055873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1}^{d(n)} a(n-k) for n ≥ 1 * where d(n) is
 *            the number of positive divisors of n
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 71, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a static cache of computed values.
 * It works for any definition of seq_t (unsigned integer types or
 * arbitrary‑precision integer classes) and for any build that
 * supplies the types idx_t (uint64_t) and seq_t.  The code uses
 * conditional compilation to handle C and C++ builds separately. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Storage for a(n) values */
#ifndef __cplusplus
static seq_t *a_vals = NULL;
static size_t a_alloc = 0;
#else
static std::vector<seq_t> a_vals;
#endif

/* Divisor‑count array – values are zero for indices that have not been
 * computed yet. */
static uint64_t *d_vals = NULL;
static uint64_t d_alloc = 0;
static idx_t max_computed = 0;

/* Ensure that the arrays are large enough for indices up to n (inclusive).
 * Newly added elements are default‑constructed or set to zero. */
static void ensure_alloc(idx_t n)
{
#ifndef __cplusplus
    if (a_alloc <= (size_t)n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_arr = (seq_t *)realloc(a_vals, new_size * sizeof(seq_t));
        if (!new_arr) return;                  /* allocation failure – ignore */
        for (size_t i = a_alloc; i < new_size; ++i) new_arr[i] = (seq_t)0;
        a_vals = new_arr;
        a_alloc = new_size;
    }
#else
    if (a_vals.size() <= (size_t)n) a_vals.resize(n + 1);
#endif

    if (d_vals == NULL) {
        d_vals = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
        if (!d_vals) return;
        memset(d_vals, 0, (n + 1) * sizeof(uint64_t));
        d_alloc = n + 1;
    } else if (d_alloc <= (size_t)n) {
        size_t new_size = (size_t)(n + 1);
        uint64_t *new_d = (uint64_t *)realloc(d_vals, new_size * sizeof(uint64_t));
        if (!new_d) return;
        memset(new_d + d_alloc, 0, (new_size - d_alloc) * sizeof(uint64_t));
        d_vals = new_d;
        d_alloc = new_size;
    }
}

/* Compute the number of positive divisors of v (v > 0). */
static uint64_t divisor_count(idx_t v)
{
    if (v == 0) return 0;
    uint64_t cnt = 0;
    idx_t lim = (idx_t)floor(sqrt((double)v));
    for (idx_t i = 1; i <= lim; ++i) {
        if (v % i == 0) {
            ++cnt;
            if (i != v / i) ++cnt;
        }
    }
    return cnt;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    ensure_alloc(n);

    /* a(0) must be 1 – initialise if still zero. */
    if (a_vals[0] == 0) a_vals[0] = (seq_t)1;

    /* Compute missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (d_vals[i] == 0) d_vals[i] = divisor_count(i);
        seq_t sum = (seq_t)0;
        uint64_t cnt = d_vals[i];
        for (uint64_t k = 1; k <= cnt; ++k) {
            sum += a_vals[i - k];
        }
        a_vals[i] = sum;
    }

    if (n > max_computed) max_computed = n;
    return a_vals[n];
}
