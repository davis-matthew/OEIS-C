/*
 * A147615  a(n) = 13 + Sum_{j=4..n+3} j!.
 * https://oeis.org/A147615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13 + Sum_{j=4}^{n+3} j! (OEIS A147615)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Static state for memoisation */
    static seq_t *fact_cache = NULL;          /* factorial values j! for j >= 0 */
    static size_t fact_cap = 0;               /* allocated size of fact_cache   */
    static seq_t *sum_cache = NULL;           /* a(n) values for n >= 0         */
    static size_t sum_cap = 0;                /* allocated size of sum_cache    */
    static idx_t last_fact_j = 3;             /* largest j for which j! known   */
    static idx_t max_n = 0;                   /* largest n for which a(n) known  */
    static seq_t current_fact;                /* j! for current last_fact_j     */
    static seq_t running_sum;                 /* Sum_{j=4}^{last_fact_j} j!      */
    static bool initialized = false;

    /* Initialisation */
    if (!initialized) {
        fact_cap = 4;                         /* need factorials 0..3 */
        fact_cache = (seq_t *)malloc(fact_cap * sizeof(seq_t));
        fact_cache[0] = (seq_t)1;
        fact_cache[1] = (seq_t)1;
        fact_cache[2] = (seq_t)2;
        fact_cache[3] = (seq_t)6;
        current_fact = fact_cache[3];

        running_sum = (seq_t)0;               /* nothing added yet          */
        sum_cap = 1;                          /* only a[0] known            */
        sum_cache = (seq_t *)malloc(sum_cap * sizeof(seq_t));
        sum_cache[0] = (seq_t)13;             /* a(0) = 13                  */
        max_n = 0;

        initialized = true;
    }

    /* Return cached value if already computed */
    if (n <= max_n) {
        return sum_cache[n];
    }

    /* Extend caches up to required n */
    idx_t target_j = n + 3;                    /* need factorial up to this j */
    for (idx_t j = last_fact_j + 1; j <= target_j; ++j) {
        /* Grow factorial array if necessary */
        if ((size_t)(j + 1) > fact_cap) {
            size_t new_cap = fact_cap * 2;
            if (new_cap < (size_t)(j + 1))
                new_cap = j + 1;
            fact_cap = new_cap;
            fact_cache = (seq_t *)realloc(fact_cache, fact_cap * sizeof(seq_t));
        }

        /* Compute next factorial */
        current_fact = current_fact * (seq_t)j;
        fact_cache[j] = current_fact;

        if (j >= 4) {
            /* Update running sum and a(n) cache */
            running_sum = running_sum + current_fact;
            idx_t n_val = j - 3;                 /* because j = n + 3   */
            if ((size_t)(n_val + 1) > sum_cap) {
                size_t new_cap = sum_cap * 2;
                if (new_cap < (size_t)(n_val + 1))
                    new_cap = n_val + 1;
                sum_cap = new_cap;
                sum_cache = (seq_t *)realloc(sum_cache, sum_cap * sizeof(seq_t));
            }
            sum_cache[n_val] = (seq_t)13 + running_sum;
            if (n_val > max_n)
                max_n = n_val;
        }
    }

    last_fact_j = target_j;
    return sum_cache[n];
}
