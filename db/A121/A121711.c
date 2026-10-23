/*
 * A121711  Lexicographically earliest permutation of the natural numbers such
 *          that gcd(s(k-1)+1, a(k)) greater than gcd(s(k-1), a(k)+1), where s
 *          is the sum of preceding terms (A121712); a(1)=1.
 * https://oeis.org/A121711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest permutation with the gcd property
   (OEIS A121711).  The function builds the sequence lazily,
   keeping all already-computed values in memory. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /*----------------------------------------*
     *  Static state for memoisation          *
     *----------------------------------------*/
    static seq_t *vals      = NULL;   /* array of sequence values      */
    static bool *used      = NULL;   /* array marking which numbers are used */
    static size_t vals_cap = 0;       /* capacity of vals array          */
    static size_t used_cap = 0;       /* capacity of used array          */
    static idx_t  computed  = 0;     /* largest n already computed      */
    static seq_t  sum       = (seq_t)0; /* sum of all computed terms       */

    /*----------------------------------------*
     *  Initialise storage on first call      *
     *----------------------------------------*/
    if (vals == NULL) {
        vals_cap = 64;
        vals = (seq_t *)malloc(sizeof(seq_t) * vals_cap);
        used_cap = 128;
        used = (bool *)calloc(used_cap, sizeof(bool));
    }

    /*----------------------------------------*
     *  Grow vals array if necessary          *
     *----------------------------------------*/
    if ((size_t)n > vals_cap) {
        while (vals_cap < (size_t)n) vals_cap <<= 1;
        vals = (seq_t *)realloc(vals, vals_cap * sizeof(seq_t));
    }

    /*----------------------------------------*
     *  Compute new terms up to n             *
     *----------------------------------------*/
    if (n > computed) {
        for (idx_t k = computed + 1; k <= n; ++k) {
            if (k == 1) {                     /*  a(1) = 1  */
                vals[0] = (seq_t)1;
                if (used_cap <= 1) {
                    used_cap <<= 1;
                    used = (bool *)realloc(used, used_cap * sizeof(bool));
                }
                used[1] = true;
                sum = (seq_t)1;
                continue;
            }

            /*  Find the smallest unused candidate that satisfies
                gcd(sum+1, a) > gcd(sum, a+1)  */
            idx_t cand = 1;
            for (;;) {
                if ((size_t)cand >= used_cap) {
                    used_cap <<= 1;
                    used = (bool *)realloc(used, used_cap * sizeof(bool));
                }
                if (!used[(size_t)cand]) {
                    seq_t cseq   = (seq_t)cand;
                    seq_t gcd1   = gcd(sum + (seq_t)1, cseq);
                    seq_t gcd2   = gcd(sum, cseq + (seq_t)1);
                    if (gcd1 > gcd2) {
                        vals[k-1] = cseq;
                        used[(size_t)cand] = true;
                        sum += cseq;
                        break;
                    }
                }
                ++cand;
            }
        }
        computed = n;
    }

    return vals[n-1];
}
