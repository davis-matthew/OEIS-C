/*
 * A214579  Number of partitions of n in which each part is divisible by the
 *          next and have no parts equal to 1.
 * https://oeis.org/A214579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n) – b(n – 1), where b(0) = 1 and b(n) = Σ_{d|n} b(d –
 *            1) for n ≥ 1
 * Range:     u8 n <= 47, u16 n <= 227, u32 n <= 1787, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for the auxiliary sequence b */
static seq_t *b_cache  = NULL;   /* array holding b[0..max]          */
static size_t b_cap    = 0;       /* allocated capacity of b_cache    */
static idx_t  b_max    = 0;       /* highest n whose b(n) is computed */
static bool   b_inited = false;   /* flag whether b[0] has been set   */

seq_t a(idx_t n)
{
    /* undefined for n < 1; return 0 for safety */
    if (n < 1) return 0;

    /* Initialize cache if not done yet */
    if (!b_inited) {
        b_cap   = 1;
        b_cache = (seq_t *)malloc(b_cap * sizeof(seq_t));
        if (!b_cache) abort();           /* out of memory */
        b_cache[0] = 1;                  /* b(0) = 1 */
        b_inited = true;
        b_max    = 0;
    }

    /* Ensure sufficient capacity for indices 0..n */
    if ((size_t)(n + 1) > b_cap) {
        size_t new_cap = b_cap ? b_cap * 2 : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(b_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        b_cache = tmp;
        b_cap   = new_cap;
    }

    /* Compute b(i) for all i in (b_max, n] */
    for (idx_t i = b_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                sum += b_cache[d - 1];
                idx_t q = i / d;
                if (q != d)
                    sum += b_cache[q - 1];
            }
        }
        b_cache[i] = sum;
    }
    b_max = n;

    /* a(n) = b(n) – b(n-1) */
    return b_cache[n] - b_cache[n - 1];
}
