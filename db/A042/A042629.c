/*
 * A042629  Denominators of continued fraction convergents to sqrt(844).
 * https://oeis.org/A042629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 21, u64 n <= 43, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1, a(1)=19, a(2)=39.
 * For n>2,
 *   a(n) = period[(n-1)%48] * a(n-1) + a(n-2),
 * where period =
 *   {19,2,1,5,1,3,1,1,1,1,1,2,6,1,7,2,3,2,2,11,4,1,3,14,3,1,4,11,2,2,3,
 *    2,7,1,6,2,1,1,1,1,1,3,1,5,1,2,19,58}.
 *
 * The sequence is the denominators of the continued‑fraction convergents
 * to √844.  The period length is 48; the first term corresponds to the
 * convergent with denominator 1.
 */

#include <stdlib.h>

/* period of the continued fraction for √844 (2a0 = 58 at the end) */
static const seq_t PERIOD[48] = {
    19, 2, 1, 5, 1, 3, 1, 1, 1, 1, 1, 2, 6, 1, 7, 2,
    3, 2, 2, 11, 4, 1, 3, 14, 3, 1, 4, 11, 2, 2, 3, 2,
    7, 1, 6, 2, 1, 1, 1, 1, 1, 3, 1, 5, 1, 2, 19, 58
};

extern seq_t a(idx_t n)
{
    /* static cache for computed denominators   */
    static seq_t *cache   = NULL;
    static idx_t  last    = 0;     /* highest index computed */
    static seq_t prev2    = 0;     /* a(-1) for the recurrence */
    static seq_t prev1    = 1;     /* a(0)  for the recurrence */

    if (n == 0) return 1;

    /* allocate cache on first use */
    if (!cache) {
        cache   = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = 1;
        last     = 0;
        /* prev2, prev1 already set to 0 and 1 */
    }

    /* return cached value if available */
    if (n <= last) {
        return cache[n];
    }

    /* expand cache to n+1 elements */
    cache = (seq_t*)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));

    /* compute numbers from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t ai = PERIOD[(i - 1) % 48];          /* a_i for recurrence */
        seq_t q  = ai * prev1 + prev2;            /* a(i) = ai*a(i-1)+a(i-2) */
        cache[i] = q;
        /* update for next iteration */
        prev2 = prev1;
        prev1 = q;
    }

    last = n;
    return cache[n];
}
