/*
 * A207592  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = -a(n-1) + 11*a(n-2) + 13*a(n-3) -
 *            18*a(n-4) - 18*a(n-5) + 6*a(n-6) + 4*a(n-7) for n > 9 with
 *            initial values a(1)=13, a(2)=169, a(3)=312, a(4)=1014,
 *            a(5)=3094, a(6)=9698, a(7)=30056, a(8)=93782, a(9)=291304 The
 *            recurrence holds for all n ≥ 10
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_PRECOMPUTE 20000          /* enough for n ≈ 10 000 */

static seq_t cache[MAX_PRECOMPUTE + 1];
static bool cache_inited = false;
static size_t cache_len = 0;          /* highest index (0‑based) that has been computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* initialise first 9 terms once */
    if (!cache_inited) {
        cache[0] = (seq_t)13;
        cache[1] = (seq_t)169;
        cache[2] = (seq_t)312;
        cache[3] = (seq_t)1014;
        cache[4] = (seq_t)3094;
        cache[5] = (seq_t)9698;
        cache[6] = (seq_t)30056;
        cache[7] = (seq_t)93782;
        cache[8] = (seq_t)291304;
        cache_len = 9;                  /* indices 0..8 are valid */
        cache_inited = true;
    }

    /* if requested value already computed, return it */
    if ((size_t)n <= cache_len) return cache[(size_t)n - 1];

    /* compute missing terms up to n using the recurrence */
    for (size_t j = cache_len; j < (size_t)n; ++j) {
        /* recurrence: a(j+1) = -a(j) + 11*a(j-1) + 13*a(j-2)
                         - 18*a(j-3) - 18*a(j-4) + 6*a(j-5) + 4*a(j-6) */
        cache[j] = -cache[j-1]
                   + (seq_t)11 * cache[j-2]
                   + (seq_t)13 * cache[j-3]
                   - (seq_t)18 * cache[j-4]
                   - (seq_t)18 * cache[j-5]
                   + (seq_t)6  * cache[j-6]
                   + (seq_t)4  * cache[j-7];
    }
    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
