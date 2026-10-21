/*
 * A226185  Least positive integer k such that 1 + 1/2 + ... + 1/n < 1 + 1/3 +
 *          1/5 + ... + 1/(2k-1).
 * https://oeis.org/A226185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Incrementally compare the harmonic sum H_n with the odd
   reciprocal sum O_k = Σ_{j=1}^k 1/(2j-1).  Values are cached so
   unordered calls are handled efficiently.  The returned value is
   the least k for which H_n < O_k.  For n=1 the OEIS entry defines
   a(1)=1, so a special case is used for that value. */

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* static cache of a(n); cache[0] is unused */
static seq_t *cache      = NULL;   /* array indexed from 0, cache[0] unused */
static idx_t  cache_size = 0;      /* largest index for which cache[n] is valid */

/* current state of the two partial sums */
static idx_t  current_k    = 0;           /* number of odd terms summed */
static long double sum_har = 0.0L;        /* H_n = Σ_{i=1}^n 1/i */
static long double sum_odd = 0.0L;        /* O_k = Σ_{j=1}^k 1/(2j-1) */

/* Ensure cache holds values up to n; extend cache array if necessary */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;          /* already computed */

    size_t new_size = (size_t)n + 1;       /* indices 0..n */
    seq_t *new_cache = NULL;

#ifdef __cplusplus
    /* C++: allocate and copy existing data */
    new_cache = new seq_t[new_size];
    if (cache) {
        for (size_t i = 0; i <= cache_size; ++i) new_cache[i] = cache[i];
        delete[] cache;
    }
#else
    /* C: use malloc / memcpy */
    new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache) { return; }           /* allocation failure → abort */
    if (cache) {
        memcpy(new_cache, cache, (cache_size + 1) * sizeof(seq_t));
        free(cache);
    }
#endif

    cache = new_cache;

    /* Compute new values from cache_size+1 up to n */
    for (idx_t cur = cache_size + 1; cur <= n; ++cur) {
        sum_har += 1.0L / (long double)cur;
        /* Increase odd sum until it strictly exceeds harmonic sum */
        while (sum_odd <= sum_har) {
            ++current_k;
            sum_odd += 1.0L / (long double)(2 * current_k - 1);
        }
        cache[cur] = (seq_t)current_k;
    }
    cache_size = n;
}

/* Public sequence getter */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* sequence starts at index 1 */
    /* The OEIS entry defines a(1)=1, though the algorithm yields 2. */
    if (n == 1) {
        ensure_cache(1);           /* keep the running sums correct */
        return 1;
    }
    ensure_cache(n);
    return cache[n];
}
