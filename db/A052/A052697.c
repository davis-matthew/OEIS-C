/*
 * A052697  Expansion of e.g.f. 1/(1-x^3-x^4).
 * https://oeis.org/A052697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function implements the recurrence for OEIS sequence A052697.
 * Recurrence (given in OEIS):
 *   a(0)=1, a(1)=0, a(2)=0, a(3)=6
 *   a(n+4) = (24 + 26*n + 9*n^2 + n^3) * a(n+1) +
 *           (24 + 50*n + 35*n^2 + 10*n^3 + n^4) * a(n)
 *
 * The implementation caches previously computed values to answer
 * any set of calls in O(n) total time, with n up to about 10,000
 * comfortably within the time limit.
 */

#include <stdlib.h>

/* External types supplied by the build system */
extern seq_t a(idx_t n);  /* Forward declaration for the compiler */

/* Internal cache for memoization */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/*
 * Ensure that the cache is large enough to contain a(n) for all n <= target.
 * If necessary, extend and compute missing values using the recurrence.
 */
static void ensure_cache(idx_t target)
{
    if (cache_len > target)          /* already computed up to target */
        return;

    size_t new_len = (size_t)(target + 1);
    /* Reallocate memory; cast void* to seq_t* for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failed */

    cache = tmp;

    /* Compute missing values from cache_len up to target */
    for (idx_t i = cache_len; i <= target; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1 || i == 2) {
            cache[i] = (seq_t)0;
        } else if (i == 3) {
            cache[3] = (seq_t)6;
        } else { /* i >= 4 */
            idx_t m = i - 4;           /* n in the recurrence */
            seq_t ms = (seq_t)m;       /* m as seq_t */
            seq_t m2 = ms * ms;        /* m^2 */
            seq_t m3 = m2 * ms;        /* m^3 */
            seq_t m4 = m3 * ms;        /* m^4 */

            seq_t coeff1 = (seq_t)24 + (seq_t)26 * ms + (seq_t)9 * m2 + m3;
            seq_t coeff2 = (seq_t)24 + (seq_t)50 * ms + (seq_t)35 * m2
                           + (seq_t)10 * m3 + m4;

            cache[i] = coeff1 * cache[i - 3] + coeff2 * cache[i - 4];
        }
    }

    cache_len = target + 1;
}

/* Public entry point used by the evaluation harness. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
