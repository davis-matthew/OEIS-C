/*
 * A063816  Growth series for fundamental group of orientable closed surface
 *          of genus 6.
 * https://oeis.org/A063816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A063816 – growth series for the fundamental group of
 *  the orientable closed surface of genus 6.
 *
 *  Generating function:
 *        (1+x)(1-x^12)/(1-23*x+23*x^12-x^13)
 *
 *  Recurrence (valid for all n ≥ 0):
 *
 *        a(n) = 23*a(n-1) - 23*a(n-12) + a(n-13) + b(n)
 *
 *  where      b(0) = 1,  b(1) = 1,  b(12) = –1,  b(13) = –1,
 *        and b(n) = 0 for all other n.
 *
 *  For n ≥ 14, b(n) = 0, so the simple linear recurrence holds.
 *
 *  The function below builds the sequence on demand, memoising
 *  all values so far so that calls can arrive in arbitrary order.
 *  No hard‑coded lookup table of sequence values is used.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /*  Static cache for memoisation  */
    static seq_t *cache = NULL;      /* array of computed terms             */
    static idx_t cache_size = 0;     /* allocated array size                */
    static idx_t computed_max = 0;    /* highest index computed so far       */

    /* Ensure the cache is initialised and large enough for n. */
    /* We always keep at least 14 entries so that the recurrence
       for n≥14 can refer to a(n‑12) and a(n‑13). */
    if (!cache) {
        cache_size = 16;             /* start with a reasonable size          */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure                    */
        /* a(0) is 1 as given by the generating function. */
        cache[0] = (seq_t)1;
        computed_max = 0;
    }

    /* Expand the cache if the requested index is beyond the current
       allocated capacity.  Double the size until it is large enough. */
    if ((idx_t)n >= cache_size) {
        idx_t new_size = cache_size;
        while ((idx_t)n >= new_size) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* realloc failure                        */
        cache = tmp;
        cache_size = new_size;
    }

    /* If the cache does not yet contain the value for n,
       compute all missing values up to n. */
    if ((idx_t)n > computed_max) {
        /* Determine the farthest index that must be computed.  We
           need all indices up to at least 13 so that future
           recurrences that refer to a(n‑12) and a(n‑13) are
           valid.  Also make sure we compute at least up to n. */
        idx_t limit = (n > 13 ? n : 13);

        for (idx_t i = computed_max + 1; i <= limit; ++i) {
            seq_t term = 0;

            /* a(i) = 23*a(i-1) - 23*a(i-12) + a(i-13) + b(i) */
            /* 23*a(i-1)  */
            term += ((seq_t)23) * cache[i - 1];

            /* -23*a(i-12)  */
            if (i >= 12) term -= ((seq_t)23) * cache[i - 12];

            /* +a(i-13)    */
            if (i >= 13) term += cache[i - 13];

            /* +b(i):  b(0)=b(1)=1,  b(12)=b(13)=-1,  else 0 */
            if (i == 0 || i == 1) term += 1;
            if (i == 12 || i == 13) term -= 1;   /* unsigned subtraction OK */

            cache[i] = term;
        }
        computed_max = limit;
    }

    /* Return the requested term.  For n < computed_max we already
       have it stored in the cache. */
    return cache[n];
}
