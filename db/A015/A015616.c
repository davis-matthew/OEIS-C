/*
 * A015616  Number of triples (i,j,k) with 1 <= i < j < k <= n and gcd(i,j,k)
 *          = 1.
 * https://oeis.org/A015616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 78, u32 n <= 2923, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=0, for n>=3
 *   a(n) = n*(n-1)*(n-2)/6 - Σ_{j=2..n} a(floor(n/j))
 */
#include <stdlib.h>

static seq_t *a_cache     = NULL;   /* cache of a(n) values, index = n */
static size_t cache_size  = 0;       /* allocated bytes for a_cache */
static idx_t  computed_up_to = 0;    /* largest n whose a(n) has been computed */

seq_t a(idx_t n)
{
    if (n <= 2U)                 /* a(1)=a(2)=0 */
        return 0;

    /* enlarge cache if necessary */
    if ((size_t)(n + 1U) > cache_size) {
        size_t new_size = (size_t)(n + 1U);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);           /* out‑of‑memory */
        /* initialise new entries to 0 */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;
        a_cache = tmp;
        cache_size = new_size;
    }

    /* compute a(k) for k = computed_up_to+1 .. n */
    for (idx_t k = computed_up_to + 1U; k <= n; ++k) {
        seq_t tri = ((seq_t)k) * ((seq_t)(k - 1)) * ((seq_t)(k - 2)) / 6;

        seq_t sum = 0;
        for (idx_t j = 2U; j <= k; ++j) {
            idx_t t = k / j;          /* floor(k/j) */
            sum += a_cache[t];
        }
        a_cache[k] = tri - sum;
    }
    computed_up_to = n;
    return a_cache[n];
}
