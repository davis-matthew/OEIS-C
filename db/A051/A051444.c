/*
 * A051444  Smallest k such that sigma(k) = n, or 0 if there is no such k,
 *          where sigma = A000203 = sum of divisors.
 * https://oeis.org/A051444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = smallest k with sigma(k)=n, or 0 if no such k exists * (σ
 *            is the sum‑of‑divisors function, A000203). The code builds a *
 *            table of σ(k) for all k up to MAX_K and records, for each n up
 *            to * MAX_N, the smallest k achieving that σ. For queries beyond
 *            MAX_N * a direct search is performed. The...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Smallest integer square root: floor(sqrt(n)) for a 64‑bit unsigned n. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32; /* sqrt(2^64-1) < 2^32 */
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

static bool tables_built = false;
static const idx_t MAX_K = 2000000;   /* maximum k to sieve (adjustable) */
static const idx_t MAX_N = 2000000;   /* maximum n for which a(n) is pre‑computed */
static seq_t *sigma_table = NULL;     /* σ(k) for 0 ≤ k ≤ MAX_K */
static seq_t *inv_table   = NULL;     /* inv_table[n] = smallest k with σ(k)=n (0 if none) */

/* Build the sigma table and the inverse mapping.  Called lazily on first use. */
static void build_tables(void)
{
    if (tables_built)
        return;
    tables_built = true;

    /* Allocate tables; abort on failure as the environment should provide enough memory. */
    sigma_table = (seq_t *)malloc((MAX_K + 1u) * sizeof(seq_t));
    inv_table   = (seq_t *)malloc((MAX_N + 1u) * sizeof(seq_t));
    if (!sigma_table || !inv_table)
        abort();

    /* initialise to zero */
    for (idx_t i = 0; i <= MAX_K; ++i)
        sigma_table[i] = (seq_t)0;
    for (idx_t i = 0; i <= MAX_N; ++i)
        inv_table[i] = (seq_t)0;

    /* Sieve σ(k) using divisor summation */
    for (idx_t d = 1; d <= MAX_K; ++d) {
        seq_t sd = (seq_t)d;
        for (idx_t k = d; k <= MAX_K; k += d) {
            sigma_table[k] += sd;
        }
    }

    /* Build inverse mapping */
    for (idx_t k = 1; k <= MAX_K; ++k) {
        seq_t s = sigma_table[k];
        if (s <= (seq_t)MAX_N && inv_table[(idx_t)s] == (seq_t)0)
            inv_table[(idx_t)s] = (seq_t)k;
    }
}

/* Direct search for σ(k)=n when n is beyond the pre‑computed range. */
static seq_t search_for_n(idx_t n)
{
    /* Enumerate k starting from 1 until a match is found. */
    for (idx_t k = 1; ; ++k) {
        seq_t sum = (seq_t)1;               /* divisor 1 */
        if (k > 1)
            sum += (seq_t)k;                /* divisor k itself */
        idx_t r = isqrt_u64(k);
        for (idx_t d = 2; d <= r; ++d) {
            if (k % d == 0) {
                sum += (seq_t)d;
                idx_t q = k / d;
                if (q != d)
                    sum += (seq_t)q;
            }
        }
        if (sum == (seq_t)n)
            return (seq_t)k;
    }
}

/* Public function required by the task. */
seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset starts at 1; return 0 for index 0 */
        return (seq_t)0;

    build_tables();

    if (n <= MAX_N)
        return inv_table[(idx_t)n];

    /* For n beyond the lookup range, fall back to direct search. */
    return search_for_n(n);
}
