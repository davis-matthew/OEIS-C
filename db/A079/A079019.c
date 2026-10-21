/*
 * A079019  Suppose p and q = p+18 are primes. Define the difference pattern
 *          of (p,q) to be the successive differences of the primes in the
 *          range p to q. There are 49 possible difference patterns, shown in
 *          the Comments line. Sequence gives smallest value of p for each
 *          difference pattern, sorted by magnitude.
 * https://oeis.org/A079019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A079019
   a(n) = the nth smallest prime p for which p and p+18 are primes
   and the successive differences of all primes in the interval
   [p , p+18] produce one of the 49 admissible patterns listed
   in the OEIS entry.  The function lazily generates the 49 minimal
   such primes on the first call, stores them in ascending order,
   and returns the (n‑th) value for arbitrary n ≥ 1.  No external
   libraries beyond those supplied in the build are used. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* small structure to hold a difference pattern (max 5 differences) */
typedef struct {
    int len;             /* number of differences in the pattern   */
    int diff[5];         /* the differences themselves             */
} Pattern;

/* The required sequence function.  External linkage. */
seq_t a(idx_t n)
{
    /* static state shared across all calls */
    static bool initialized = false;
    static Pattern patterns[49];
    static seq_t   solutions[49];
    static int pat_cnt = 0;   /* number of distinct patterns discovered */

    /* ==============================================================
       Lazy generation of the 49 minimal solutions
       ==============================================================
       Run only once.  Subsequent calls just return the cached value. */
    if (!initialized) {
        uint64_t p = 2;                /* start from the first prime */

        while (pat_cnt < 49) {
            /* skip composite numbers */
            while (!is_prime(p)) ++p;

            uint64_t q = p + 18;
            if (!is_prime(q)) { ++p; continue; }

            /* collect all primes between p and q inclusive */
            uint64_t primes[6];
            int primes_cnt = 0;
            for (uint64_t x = p; x <= q; ++x) {
                if (is_prime(x))
                    primes[primes_cnt++] = x;
            }

            /* we must have at least two primes (p and q) */
            if (primes_cnt < 2 || primes[primes_cnt-1] != q) { ++p; continue; }

            /* compute the difference pattern */
            int dlen = primes_cnt - 1;
            int diffs[5];
            for (int i = 0; i < dlen; ++i)
                diffs[i] = (int)(primes[i+1] - primes[i]);

            /* check if this pattern is already known */
            bool seen = false;
            for (int i = 0; i < pat_cnt; ++i) {
                if (patterns[i].len != dlen) continue;
                int j;
                for (j = 0; j < dlen; ++j)
                    if (patterns[i].diff[j] != diffs[j])
                        break;
                if (j == dlen) { seen = true; break; }
            }

            if (!seen) {
                /* new pattern – record minimal prime p */
                patterns[pat_cnt].len = dlen;
                for (int j = 0; j < dlen; ++j)
                    patterns[pat_cnt].diff[j] = diffs[j];
                solutions[pat_cnt] = (seq_t)p;
                ++pat_cnt;
            }

            ++p;   /* move to the next candidate */
        }

        /* sort the 49 solutions in ascending order using insertion sort
           (the number is tiny, so a simple sort is adequate). */
        for (idx_t i = 1; i < (idx_t)pat_cnt; ++i) {
            seq_t key = solutions[i];
            idx_t j = i;
            while (j > 0 && key < solutions[j-1]) {
                solutions[j] = solutions[j-1];
                --j;
            }
            solutions[j] = key;
        }

        initialized = true;
    }

    /* --------------------------------------------------------------
       Answer queries after caching is ready.
       -------------------------------------------------------------- */
    if (n < 1 || n > (idx_t)pat_cnt) return 0;
    return solutions[n-1];
}
