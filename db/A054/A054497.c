/*
 * A054497  Number of symmetric nonnegative integer 7 X 7 matrices with sum of
 *          elements equal to 4*n, under action of dihedral group D_4.
 * https://oeis.org/A054497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 7a(n‑1) – 18a(n‑2)
 *            + 14a(n‑3) + 25a(n‑4) * – 63a(n‑5) + 36a(n‑6) + 36a(n‑7) –
 *            63a(n‑8) * + 25a(n‑9) + 14a(n‑10) – 18a(n‑11) + 7a(n‑12) –
 *            a(n‑13) * with initial values: * a(0)=1, a(1)=7, a(2)=31,
 *            a(3)=105, a(4)=300, a(5)=756, * a(6)=1732, a(7)=3676, a(...
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 55, u64 n <= 715,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial 13 terms (indices 0 .. 12).  They are needed both for
     * direct queries and for building further terms. */
    static const seq_t init[13] = {
        (seq_t)1, (seq_t)7,  (seq_t)31,  (seq_t)105,
        (seq_t)300, (seq_t)756,  (seq_t)1732, (seq_t)3676,
        (seq_t)7330, (seq_t)13870,(seq_t)25102,(seq_t)43714,
        (seq_t)73612
    };

    /* Cache for memoised values.  The cache grows as larger indices are
     * requested.  The cache holds values a(0) .. a(computed_max).  */
    static seq_t *cache = NULL;      /* pointer to the cached values */
    static idx_t  cache_size = 0;    /* allocated size (elements)     */
    static idx_t  computed_max = 0;  /* highest n for which a(n) is known */

    /* For very small indices we can return immediately. */
    if (n < 13)
        return init[n];

    /* Initialise or grow the caching structure.  On the first call we
     * allocate space for at least the 13 initial terms.  On subsequent
     * calls we increase the allocation if needed. */
    if (cache == NULL) {
        cache_size   = (n > 12 ? n + 1 : 13);
        cache        = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) exit(1);          /* OOM condition */
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = init[i];
        computed_max = 12;                   /* we know a(0)..a(12)   */
    } else if (cache_size <= n) {
        /* Need more space */
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) exit(1);          /* OOM condition */
        cache_size = new_size;
    }

    /* Compute missing terms up to n.  We only iterate from the last
     * computed index + 1 up to the requested index.  The recurrence
     * uses only the previous 13 terms, so we can build the sequence
     * iteratively. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* Compute the positive part and the negative part
         * separately to avoid signed overflow when using unsigned
         * arithmetic. */
        seq_t pos = 7 * cache[i - 1]
                  + 14 * cache[i - 3]
                  + 25 * cache[i - 4]
                  + 36 * cache[i - 6]
                  + 36 * cache[i - 7]
                  + 25 * cache[i - 9]
                  + 14 * cache[i - 10]
                  + 7  * cache[i - 12];

        seq_t neg = 18 * cache[i - 2]
                  + 63 * cache[i - 5]
                  + 63 * cache[i - 8]
                  + 18 * cache[i - 11]
                  +    cache[i - 13];

        cache[i] = pos - neg;
    }

    computed_max = n;

    return cache[n];
}
