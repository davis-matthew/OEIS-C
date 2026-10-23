/*
 * A152873  Number of permutations of {1,2,...,n} (n>=2) having a single run
 *          of even entries. For example, the permutation 513284679 has a
 *          single run of even entries: 2846.
 * https://oeis.org/A152873
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if (n even) ((n/2)+1)*((n/2)!)^2 else ((n-1)/2)! *
 *            ((n+1)/2+1)! (i.e. k!*(k+2)! with n=2k+1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* for malloc, realloc */

static seq_t *fact_cache = NULL;   /* factorial cache: fact_cache[i] = i!  */
static size_t fact_cache_len = 0;  /* number of computed factorials  */
static size_t fact_cache_cap = 0;  /* allocated capacity               */

/* Ensure factorials up to m are computed and cached */
static void ensure_fact(idx_t m) {
    size_t needed = (size_t)m + 1;          /* need indices 0..m inclusive */
    if (fact_cache == NULL) {               /* first time: allocate minimal */
        fact_cache_cap = 8;
        fact_cache = (seq_t *)malloc(fact_cache_cap * sizeof(seq_t));
        if (!fact_cache) {                  /* allocation failed, abort */
            abort();
        }
        fact_cache[0] = 1;                  /* 0! = 1 */
        fact_cache_len = 1;
    }
    if (fact_cache_cap < needed) {
        size_t new_cap = fact_cache_cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
        if (!tmp) { abort(); }
        fact_cache = tmp;
        fact_cache_cap = new_cap;
    }
    for (size_t i = fact_cache_len; i <= (size_t)m; ++i) {
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;
    }
    fact_cache_len = needed;
}

seq_t fact_upto(idx_t n) {
    ensure_fact(n);
    return fact_cache[(size_t)n];
}

seq_t a(idx_t n) {
    if (n < 2) return 0;   /* behaviour below offset is unspecified */

    if ((n & 1) == 0) {          /* n is even */
        idx_t k = n >> 1;         /* k = n/2 */
        seq_t fact_k = fact_upto(k);
        seq_t prod = fact_k * fact_k;         /* (k!)^2 */
        prod *= (seq_t)(k + 1);               /* *(k+1) */
        return prod;
    } else {                     /* n is odd */
        idx_t k = (n - 1) >> 1;   /* k = (n-1)/2 */
        seq_t fact_k   = fact_upto(k);
        seq_t fact_k2  = fact_upto(k + 2);    /* (k+2)! */
        return fact_k * fact_k2;
    }
}
