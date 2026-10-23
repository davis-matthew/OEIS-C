/*
 * A136767  n! never ends in this many 0's in base 4.
 * https://oeis.org/A136767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th positive integer m such that no integer k ≥ 1
 *            satisfies floor(v₂(k!)/2) = m; i.e., m is not the number of
 *            trailing zeros of any factorial in base 4
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static data for memoization */
    static idx_t fact_n = 0;              /* k for which k! has been processed */
    static idx_t v2    = 0;              /* exponent of 2 in fact_n! */
    static unsigned char *seen = NULL;    /* seen[t] == 1 if t has appeared as floor(v2/2) */
    static size_t seen_cap = 0;
    static seq_t *missing = NULL;         /* list of missing t values */
    static size_t miss_cap = 0, miss_len = 0;
    static idx_t next_scan = 0;   /* next t to check for missing */

    /* lazily create missing buffer */
    if (miss_cap == 0) {
        miss_cap = 64;
        missing = (seq_t *)malloc(miss_cap * sizeof(seq_t));
        if (!missing) return 0; /* allocation failure, return 0 as sentinel */
    }

    /* loop until we have at least n missing values */
    while (miss_len < n) {
        /* advance factorial */
        fact_n++;
        idx_t x = fact_n;
        idx_t add = 0;
        /* count factors of 2 in fact_n */
        while ((x & 1) == 0) {
            add++;
            x >>= 1;
        }
        v2 += add;

        /* number of trailing zeros of fact_n! in base 4 */
        idx_t t = v2 >> 1;

        /* grow seen array if necessary */
        if ((size_t)t >= seen_cap) {
            size_t new_cap = (size_t)t + 1;
            unsigned char *tmp = (unsigned char *)realloc(seen, new_cap);
            if (!tmp) return 0; /* allocation failure */
            /* initialize newly allocated part to 0 */
            memset(tmp + seen_cap, 0, new_cap - seen_cap);
            seen = tmp;
            seen_cap = new_cap;
        }
        seen[(size_t)t] = 1;

        /* scan newly reachable t values for missing numbers */
        while (next_scan <= t) {
            if (!seen[(size_t)next_scan]) {
                /* add to missing list, resize if needed */
                if (miss_len == miss_cap) {
                    size_t new_cap = miss_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(missing, new_cap * sizeof(seq_t));
                    if (!tmp) return 0; /* allocation failure */
                    missing = tmp;
                    miss_cap = new_cap;
                }
                missing[miss_len++] = (seq_t)next_scan;
            }
            next_scan++;
        }
    }

    return missing[n - 1];
}
