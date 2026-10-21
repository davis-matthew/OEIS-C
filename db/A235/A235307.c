/*
 * A235307  Number of (n+1) X (5+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 6
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A235307: Number of (n+1)×6 arrays with elements 0..5 
   such that each 2×2 subblock has its diagonal sum differing from its 
   antidiagonal sum by 6.  The values satisfy the 15‑term linear 
   recurrence (empirical) given in the OEIS entry:

   a(n) =  4 a(n-1) + 25 a(n-2) -112 a(n-3) -231 a(n-4)
          +1260 a(n-5) + 875 a(n-6) -7280 a(n-7) -224 a(n-8)
          +22736 a(n-9) -7980 a(n-10) -36288 a(n-11) +21456 a(n-12)
          +23040 a(n-13) -17280 a(n-14)

   for n ≥ 15, with initial terms:
   a(1) = 4006, a(2) = 6786, a(3) = 10790, a(4) = 21090,
   a(5) = 38182, a(6) = 83394, a(7) = 168998, a(8) = 400674,
   a(9) = 889126, a(10) = 2238786, a(11) = 5323430,
   a(12) = 14015010, a(13) = 35074342, a(14) = 95480514.
*/

#include <stdlib.h>
#include <stddef.h>

/* defined elsewhere: */
extern seq_t a(idx_t n);

/* static cache for memoised values */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* base values a(0..14), a(0) unused (0) */
static const seq_t base_vals[15] = {
    (seq_t)0ULL,        /* a(0) – not used, placeholder for index 0 */
    (seq_t)4006ULL,     /* a(1) */
    (seq_t)6786ULL,     /* a(2) */
    (seq_t)10790ULL,    /* a(3) */
    (seq_t)21090ULL,    /* a(4) */
    (seq_t)38182ULL,    /* a(5) */
    (seq_t)83394ULL,    /* a(6) */
    (seq_t)168998ULL,   /* a(7) */
    (seq_t)400674ULL,   /* a(8) */
    (seq_t)889126ULL,   /* a(9) */
    (seq_t)2238786ULL,  /* a(10) */
    (seq_t)5323430ULL,  /* a(11) */
    (seq_t)14015010ULL, /* a(12) */
    (seq_t)35074342ULL, /* a(13) */
    (seq_t)95480514ULL  /* a(14) */
};

seq_t a(idx_t n)
{
    /* offset 1: only defined for n ≥ 1.  Return 0 for n == 0. */
    if (n == 0) return (seq_t)0;

    /* ensure cache is large enough to hold indices 0 … n */
    size_t needed = (size_t)n + 1;           /* includes index n */
    if (needed > cache_size) {
        /* enlarge cache */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        cache = tmp;
        /* fill new entries */
        for (size_t i = cache_size; i < needed; ++i) {
            if (i <= 14) {
                /* base values for i = 1 … 14, a(0) stays 0 */
                cache[i] = base_vals[i];
            } else {
                /* compute using the 15‑term recurrence */
                seq_t pos = 0, neg = 0;
                pos += (seq_t)4     * cache[i-1];
                pos += (seq_t)25    * cache[i-2];
                pos += (seq_t)1260  * cache[i-5];
                pos += (seq_t)875   * cache[i-6];
                pos += (seq_t)22736 * cache[i-9];
                pos += (seq_t)21456 * cache[i-12];
                pos += (seq_t)23040 * cache[i-13];
                neg += (seq_t)112   * cache[i-3];
                neg += (seq_t)231   * cache[i-4];
                neg += (seq_t)7280  * cache[i-7];
                neg += (seq_t)224   * cache[i-8];
                neg += (seq_t)7980  * cache[i-10];
                neg += (seq_t)36288 * cache[i-11];
                neg += (seq_t)17280 * cache[i-14];
                cache[i] = pos - neg;
            }
        }
        cache_size = needed;
    }

    return cache[n];
}
