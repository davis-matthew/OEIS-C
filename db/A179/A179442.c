/*
 * A179442  a(n) = ((n-1)! * (n+1)!) / n.
 * https://oeis.org/A179442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-1)! * (n+1)!) / n (offset 1)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* factorial cache: fact_cache[i] = i! for i = 0 .. fact_len-1   */
static seq_t *fact_cache = NULL;
static size_t  fact_cap  = 0;   /* capacity of fact_cache (elements) */
static idx_t   fact_len  = 1;   /* number of factorials currently stored (fact_len >= 1) */

seq_t a(idx_t n)
{
    /* -------------- ensure factorial cache holds (n-1)! -------------- */
    if (!fact_cache) {                   /* first time: allocate */
        fact_cap  = 16;
        fact_cache = (seq_t*)realloc(NULL, (fact_cap + 1) * sizeof(seq_t));
        if (!fact_cache) return (seq_t)0;   /* allocation failed */
        fact_cache[0] = (seq_t)1;           /* 0! = 1 */
        fact_len = 1;
    }

    while (fact_len <= n-1) {             /* need to extend cache */
        /* grow capacity */
        size_t new_cap = fact_cap ? fact_cap * 2 : 16;
        while (new_cap <= n-1) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(fact_cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        fact_cache = tmp;
        fact_cap   = new_cap;
        /* keep existing entries, new tail will be filled below */

        /* compute missing factorials up to n-1 */
        for (idx_t i = fact_len; i <= n-1; ++i) {
            fact_cache[i] = fact_cache[i-1] * (seq_t)i;
        }
        fact_len = n;                         /* now 0! .. (n-1)! present */
    }

    /* -------------- compute a(n) using (n-1)! -------------- */
    seq_t f = fact_cache[n-1];                 /* (n-1)!  */
    return f * f * ((seq_t)n + 1);             /* f^2 * (n+1)  */
}
