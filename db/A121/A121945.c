/*
 * A121945  a(n) is the sum of the first n factorials in decreasing powers
 *          from n to 1. a(n) = Sum_{k = 1..n} k!^(n-k+1).
 * https://oeis.org/A121945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k = 1..n} (k!)^(n-k+1) (offset 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper macro for casting realloc to the appropriate type in C or C++ */
#ifdef __cplusplus
#  define RELOC(ptr, size) reinterpret_cast<seq_t*>(realloc(ptr, size))
#else
#  define RELOC(ptr, size) ((seq_t*)realloc(ptr, size))
#endif

seq_t a(idx_t n)
{
    /* Static state for memoization */
    static idx_t      current_n = 0;   /* highest n that has been computed */
    static seq_t    *fact      = NULL;/* factorials: fact[i] = i!   */
    static size_t     fact_cap  = 0;   /* allocated size of fact   */
    static seq_t    *term      = NULL;/* term[k] = k!^(current_n-k+1) */
    static size_t     term_cap  = 0;   /* allocated size of term   */
    static seq_t    *cache     = NULL;/* cached a(k) values        */
    static size_t     cache_cap = 0;   /* allocated size of cache   */
    static seq_t      sum       = 0;   /* current running sum      */

    if (n == 0)
        return (seq_t)0;               /* index 0 is not used */

    /* Ensure factorial array covers [0..n] */
    if (fact == NULL) {
        fact_cap = (size_t)n + 1;
        fact = (seq_t*)malloc(fact_cap * sizeof(seq_t));
        if (!fact) exit(1);
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i <= n; ++i)
            fact[i] = fact[i-1] * (seq_t)i;
    } else if (fact_cap <= n) {
        size_t old_cap = fact_cap;
        fact_cap = (size_t)n + 1;
        fact = RELOC(fact, fact_cap * sizeof(seq_t));
        if (!fact) exit(1);
        for (idx_t i = old_cap; i <= n; ++i)
            fact[i] = fact[i-1] * (seq_t)i;
    }

    /* Ensure term array covers [1..n] (index 0 unused) */
    if (term == NULL || term_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        term = RELOC(term, new_cap * sizeof(seq_t));
        if (!term) exit(1);
        term_cap = new_cap;
    }

    /* Ensure cache array covers [0..n] (index 0 unused) */
    if (cache == NULL || cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        cache = RELOC(cache, new_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache_cap = new_cap;
    }

    /* Compute forward until we reach requested n */
    while (current_n < n) {
        idx_t next = current_n + 1;   /* next n to compute */

        if (current_n == 0) {
            /* Base case: n = 1 */
            term[1] = fact[1];            /* 1!^1 */
            sum = term[1];
            cache[1] = sum;
        } else {
            /* Update existing terms by multiplying by their factorials */
            for (idx_t k = 1; k <= current_n; ++k) {
                seq_t old = term[k];
                seq_t new_val = old * fact[k];
                term[k] = new_val;
                sum += new_val - old;  /* add difference */
            }
            /* Add the new term k = next: (next)!^1 = fact[next] */
            term[next] = fact[next];
            sum += term[next];
            cache[next] = sum;
        }

        current_n = next;
    }

    return cache[n];
}
