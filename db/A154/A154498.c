/*
 * A154498  Sum of any 3 consecutive numbers is prime, a(1)=41,a(2)=43.
 * https://oeis.org/A154498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function generates OEIS sequence A154498.
   The sequence is defined as the list of primes a(n) (n≥1) 
   such that a(n) is the smallest prime greater than a(n‑1)
   with the property that a(n‑2)+a(n‑1)+a(n) is also prime.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>          /* for uint64_t (idx_t) */
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* cache[0] == a(1), cache[1] == a(2), ... */
    static seq_t *cache = NULL;      /* dynamic array of computed terms */
    static idx_t cached = 0;         /* number of terms stored */
    static size_t cap = 0;           /* allocated capacity */

    if (n == 0)                      /* defined only for n>=1 */
        return 0;

    /* enlarge cache if necessary */
    if (cached < n) {
        if (cap < (size_t)n) {
            cap = (size_t)n;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (!cache) abort();    /* allocation failed */
        }

        for (idx_t i = cached; i < n; ++i) {
            seq_t val;
            if (i == 0) {            /* a(1) */
                val = 41;
            } else if (i == 1) {     /* a(2) */
                val = 43;
            } else {
                seq_t prev1 = cache[i - 1];   /* a(n‑1) */
                seq_t prev2 = cache[i - 2];   /* a(n‑2) */

                seq_t cand = prev1 + 2;       /* next odd number */
                for (;;) {
                    if (!is_prime_big(cand)) {
                        cand += 2;            /* skip composites */
                        continue;
                    }
                    seq_t sum = prev2 + prev1 + cand;
                    if (is_prime_big(sum)) {
                        val = cand;
                        break;
                    }
                    cand += 2;                /* next odd prime candidate */
                }
            }
            cache[i] = val;
        }
        cached = n;
    }

    return cache[n - 1];   /* return a(n) */
}
