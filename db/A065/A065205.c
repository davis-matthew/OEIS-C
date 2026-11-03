/*
 * A065205  Number of subsets of proper divisors of n that sum to n.
 * https://oeis.org/A065205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of proper divisors of n that sum to n
 * Range:     u8 n <= 119, u16 n <= 719, u32 n <= 2519, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 8201 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Returns the number of subsets of the proper divisors of n
   that sum to n.  Proper divisors are all positive divisors of n
   except n itself.  Each divisor may be used at most once.  */
seq_t a(idx_t n)
{
    /* Collect the proper divisors of n into a small static array. */
    idx_t divs[128];
    size_t divcnt = 0;
    for (idx_t i = 1; (i * i) <= n; ++i)
    {
        if (n % i == 0)
        {
            if (i != n)
                divs[divcnt++] = i;
            idx_t j = n / i;
            if (j != i && j != n)
                divs[divcnt++] = j;
        }
    }

    /* Dynamic array for DP: dp[s] = number of subsets of the
       processed divisors that sum to s.  We re-use the same
       buffer across calls for efficiency. */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;
    ensure_capacity(n + 1, &dp_cap, OEISC_VPP(&dp), sizeof(seq_t));

    /* Initialize DP.  dp[0] = 1 (empty set), all others 0. */
    dp[0] = 1;
    for (size_t s = 1; s <= n; ++s)
        dp[s] = 0;

    /* Subset-sum DP over the proper divisors. */
    for (size_t idx = 0; idx < divcnt; ++idx)
    {
        idx_t d = divs[idx];
        for (size_t sum = n; sum >= (size_t)d; --sum)
            dp[sum] = dp[sum] + dp[sum - (size_t)d];
    }

    return dp[n];
}
