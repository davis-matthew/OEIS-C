/*
 * A014138  Partial sums of (Catalan numbers starting 1, 2, 5, ...).
 * https://oeis.org/A014138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=1..n} Catalan(k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#if defined(__cplusplus)
#include <type_traits>
#endif

#if defined(__cplusplus)
/* Helper that performs a*num/den using 128‑bit precision for integral
   seq_t, otherwise uses standard big‑integer operators. */
static inline seq_t mul_div_seq(const seq_t& a, uint64_t num, uint64_t den)
{
    if constexpr (std::is_integral_v<seq_t>) {
        return (seq_t)(((__int128)a * num) / den);
    } else {
        return (a * num) / den;
    }
}
#else
/* In plain C all seq_t types are integral; use 128‑bit to avoid overflow. */
static inline seq_t mul_div_seq(seq_t a, uint64_t num, uint64_t den)
{
    return (seq_t)(((__int128)a * num) / den);
}
#endif

seq_t a(idx_t n)
{
    /* Static cache: cache[i] = a(i) for i <= last, initially empty. */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated number of entries */
    static idx_t last = 0;          /* highest index computed */
    static seq_t last_cat = (seq_t)1;/* Catalan(last) for recurrence */
    static seq_t sum = 0;           /* partial sum up to last */

    if (n == 0) return 0;

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        cache[0] = 0;               /* a(0) = 0 */
        last = 0;
        last_cat = (seq_t)1;        /* Catalan(0) = 1 */
        sum = 0;
    }

    /* If requested value already known, return it. */
    if (n <= last && cache != NULL) {
        return cache[n];
    }

    /* Ensure enough space for new entries. */
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t idx = last + 1; idx <= n; ++idx) {
        /* Catalan recurrence: C_k = C_{k-1} * (4*k-2) / (k+1) */
        seq_t new_cat = mul_div_seq(last_cat, (4ULL * idx) - 2ULL, idx + 1ULL);
        sum += new_cat;
        cache[idx] = sum;
        last_cat = new_cat;
    }

    last = n;
    return cache[n];
}
